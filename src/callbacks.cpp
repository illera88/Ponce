//! \file
/*
**  Copyright (c) 2016 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

#include <list>
#include <locale> 
#include <fstream>

// Ponce
#include "callbacks.hpp"
#include "globals.hpp"
#include "context.hpp"
#include "utils.hpp"
#include "blacklist.hpp"
#include "actions.hpp"
#include "triton_logic.hpp"

//IDA
#include <ida.hpp>
#include <dbg.hpp>
#include <loader.hpp>
#include <intel.hpp>
#include <bytes.hpp>

//Triton
#include "triton/api.hpp"
#include "triton/x86Specifications.hpp"

ssize_t idaapi tracer_callback(void* user_data, int notification_code, va_list va)
{
    //if (cmdOptions.showExtraDebugInfo)
    //    msg("[+] Notification code: %d str: %s\n", notification_code, notification_code_to_string(notification_code).c_str());
    switch (notification_code)
    {
    case dbg_process_start:
    case dbg_process_attach:
    {
        if (cmdOptions.showDebugInfo)
            msg("[+] Starting the debugged process. Reseting all the engines.\n");
        triton_restart_engines();        
        break;
    }
    case dbg_step_into:
    case dbg_step_over:
    {
        ponce_runtime_status.tracing_start_time = 0;
        break;
    }
    case dbg_trace:
    {
        //We only want to analyze the thread being analyzed
        if (ponce_runtime_status.analyzed_thread != get_current_thread())
            break;
        //If the trigger is disbaled then the user is manually stepping with the ponce tracing disabled
        if (!ponce_runtime_status.runtimeTrigger.getState())
            break;

        thid_t tid = va_arg(va, thid_t);
        ea_t pc = va_arg(va, ea_t);
        //Sometimes the cmd structure doesn't correspond with the traced instruction
        //With this we are filling cmd with the instruction at the address specified

        if (should_blacklist(pc, tid)) {
            // We have blacklisted this call so we should not keep going 
            return 0;
        }

        //If the instruciton is not a blacklisted call we analyze the instruction
        //We don't want to reanalize instructions. p.e. if we put a bp we receive two events, the bp and this one
        if (ponce_runtime_status.last_triton_instruction == NULL || (ponce_runtime_status.last_triton_instruction != NULL && ponce_runtime_status.last_triton_instruction->getAddress() != pc))
            tritonize(pc, tid);

        ponce_runtime_status.current_trace_counter++;
        ponce_runtime_status.total_number_traced_ins++;
        //Every 1000 traced instructions we show with debug that info in the output
        if (cmdOptions.showDebugInfo && ponce_runtime_status.total_number_traced_ins % 1000 == 0)
            msg("Instructions traced: %d Symbolic instructions: %d Symbolic conditions: %d Time: %lld secs\n", ponce_runtime_status.total_number_traced_ins, ponce_runtime_status.total_number_symbolic_ins, ponce_runtime_status.total_number_symbolic_conditions, GetTimeMs64() - ponce_runtime_status.tracing_start_time);
        //msg("[+] Instructions traced: %d\n", ponce_runtime_status.total_number_traced_ins);

        //This is the wow64 switching, we need to skip it. https://forum.hex-rays.com/viewtopic.php?f=8&t=4070
        if (ponce_runtime_status.last_triton_instruction->getDisassembly().find("call dword ptr fs:[0xc0]") != -1) {
            if (cmdOptions.showExtraDebugInfo)
                msg("[+] Wow64 switching! Requesting a step_over\n");
            //And now we need to stop the tracing, do step over and reenable the tracing...
            //disable_step_trace();
            suspend_process();
            //We don't want to do a real step over (it would reset the timer)
            ponce_runtime_status.ignore_wow64_switching_step = true;
            request_step_over();
            request_continue_process();
            run_requests();
            break;
        }

        //Check if the limit instructions limit was reached
        if (cmdOptions.limitInstructionsTracingMode && ponce_runtime_status.current_trace_counter >= cmdOptions.limitInstructionsTracingMode) {
            int answer = ask_yn(1, "[?] %u instructions has been traced. Do you want to execute %u more?", ponce_runtime_status.total_number_traced_ins, (unsigned int)cmdOptions.limitInstructionsTracingMode);
            if (answer == 0 || answer == -1)  { //No or Cancel
                // stop the trace mode and suspend the process
                disable_step_trace();
                suspend_process();
                msg("[!] Process suspended (Traced %d instructions)\n", ponce_runtime_status.total_number_traced_ins);
            }
            else {
                ponce_runtime_status.current_trace_counter = 0;
            }
        }

        //Check if the time limit for tracing was reached
        if (cmdOptions.limitTime != 0) {
            //This is the first time we start the tracer
            if (ponce_runtime_status.tracing_start_time == 0) {
                ponce_runtime_status.tracing_start_time = GetTimeMs64();
            }
            else if ((GetTimeMs64() - ponce_runtime_status.tracing_start_time) / 1000 >= cmdOptions.limitTime) {
                int answer = ask_yn(1, "[?] the tracing was working for %u seconds(%u inst traced!). Do you want to execute it %u more?", (unsigned int)((GetTimeMs64() - ponce_runtime_status.tracing_start_time) / 1000), ponce_runtime_status.total_number_traced_ins, (unsigned int)cmdOptions.limitTime);
                if (answer == 0 || answer == -1) //No or Cancel
                {
                    // stop the trace mode and suspend the process
                    disable_step_trace();
                    suspend_process();
                    msg("[!] Process suspended (Traced %d instructions)\n", ponce_runtime_status.total_number_traced_ins);
                }
                else
                {
                    ponce_runtime_status.tracing_start_time = GetTimeMs64();
                }
            }
        }
        break;
    }
    case dbg_bpt:
    {
        //We only want to analyze the thread being analyzed
        if (ponce_runtime_status.analyzed_thread != get_current_thread())
            break;
        msg("BP Instructions traced: %d Symbolic instructions: %d Symbolic conditions: %d Time: %lld secs\n", ponce_runtime_status.total_number_traced_ins, ponce_runtime_status.total_number_symbolic_ins, ponce_runtime_status.total_number_symbolic_conditions, GetTimeMs64() - ponce_runtime_status.tracing_start_time);

        thid_t tid = va_arg(va, thid_t);
        ea_t pc = va_arg(va, ea_t);
        int* warn = va_arg(va, int*);
        //This variable defines if a breakpoint is a user-defined breakpoint or not
        bool user_bp = true;
        //We look if there is a pending action for this breakpoint
        for (auto it = breakpoint_pending_actions.begin(); it != breakpoint_pending_actions.end(); ++it)
        {
            breakpoint_pending_action bpa = *it;
            //If we find a pendign action we execute the callback
            if (pc == bpa.address)
            {
                bpa.callback(pc);
                tritonize(pc, tid);

                //The pending breakpoints are used for enable the tracing so we consider this instruction tracing too
                ponce_runtime_status.current_trace_counter++;
                ponce_runtime_status.total_number_traced_ins++;
                //If there is a user-defined bp in the same address we should respect it and dont continue the exec
                if (!bpa.ignore_breakpoint)
                {
                    //If it's a breakpoint the plugin set not a user-defined bp
                    user_bp = false;
                    //If not this is the bp we set to taint the arguments, we should rmeove it and continue the execution
                    del_bpt(pc);
                    enable_step_trace(true);
                    //We dont want to skip library funcions or debug segments
                    set_step_trace_options(0);
                    continue_process();
                    //We delete the comment
                    ponce_set_cmt(pc, "", false);
                    breakpoint_pending_actions.erase(it);
                }
                break;
            }
        }
        //If it is a user break point we enable again the step tracing if it was enabled previously...
        //The idea is if the user uses Execute native til next bp, and IDA reachs the next bp we reenable the tracing
        if (user_bp)
        {
            ponce_runtime_status.tracing_start_time = 0;
            //request_suspend_process();
            //run_requests();
            //disable_step_trace();
            //request_enable_step_trace();
            //If the trigger is disabled then the user is manually stepping with the ponce tracing disabled
            //if (ponce_runtime_status.runtimeTrigger.getState())
            //enable_step_trace(ponce_runtime_status.runtimeTrigger.getState());
        }
        break;
    }
    case dbg_process_exit:
    {
        if (cmdOptions.showDebugInfo)
            msg("[!] Process_exiting...\n");
        //Do we want to unhook this event? I don't think so we want to be hooked for future sessions
        //unhook_from_notification_point(HT_DBG, tracer_callback, NULL);
        ponce_runtime_status.runtimeTrigger.disable();
        enable_step_trace(false);
        //Removing snapshot if it exists
        if (snapshot.exists())
            snapshot.resetEngine();
        break;
    }
    }
    return 0;
}

//---------------------------------------------------------------------------
// Callback for ui notifications
ssize_t idaapi ui_callback(void* ud, int notification_code, va_list va)
{
    switch (notification_code)
    {
        // Called when IDA is preparing a context menu for a view
        // Here dynamic context-depending user menu items can be added.
    case ui_populating_widget_popup:
    {
        msg("at ui_populating_widget_popup\n");
        TWidget* form = va_arg(va, TWidget*);
        TPopupMenu* popup_handle = va_arg(va, TPopupMenu*);
        int view_type = get_widget_type(form);
        bool success;

        // Set the name for the action depending if using tainting or symbolic engine
        if (cmdOptions.use_tainting_engine) {
            action_list[2].menu_path = TAINT;
            action_list[3].menu_path = TAINT;
        }
        else {
            action_list[2].menu_path = SYMBOLIC;
            action_list[3].menu_path = SYMBOLIC;
        }

        //Adding a separator
        success = attach_action_to_popup(form, popup_handle, "", SETMENU_INS);

        /*Iterate over all the actions*/
        for (int i = 0;; i++) {
            if (action_list[i].action_decs == NULL)
                break;

            if (cmdOptions.use_tainting_engine && strcmp(action_list[i].menu_path, "SMT Solver/") == 0) {
                /* Do not populate SMT Solver options if we use the tainting engine*/
                continue;
            }

            /*Iterate over the view types of every action*/
            for (int j = 0;; j++) {
                if (action_list[i].view_type[j] == __END__) {
                    break;
                }
                if (action_list[i].view_type[j] == view_type) {  
                    success = attach_action_to_popup(form, popup_handle, action_list[i].action_decs->name, action_list[i].menu_path, SETMENU_INS);
                    msg("name: %s menu path: %s success: %s\n", action_list[i].action_decs->name, action_list[i].menu_path, success ? "true" : "false");
                }
            }
        }   

        //Adding a separator
        success = attach_action_to_popup(form, popup_handle, "", SETMENU_INS);

        ea_t cur_ea = get_screen_ea();

        /* Here we fill the posible SMT branches to solve if there is multiple*/
        if (view_type == BWN_DISASM &&
            !(is_debugger_on() && !ponce_runtime_status.runtimeTrigger.getState())) { // Don't let solve formulas if user is debugging natively 

            /* For the selected address(cur_ea), let's count how many branches we can reach (how many non taken addresses are in reach)*/
            int non_taken_branches_n = std::count_if(api.getPathConstraints().begin(), api.getPathConstraints().end(), [cur_ea](const auto& pc) {
                for (auto const& [taken, srcAddr, dstAddr, pc] : pc.getBranchConstraints()) {
                    if (cur_ea == srcAddr && !taken) return true;
                }
                return false;
                });


            if (non_taken_branches_n == 0) {
                // Disabled menu (so the user knows it's an option in some cases), already registered, we just need to attach it to the popup
                attach_action_to_popup(form, popup_handle, action_IDA_solve_formula_sub.name, "SMT Solver/Solve formula", SETMENU_INS);
            }
            else if (non_taken_branches_n == 1) {
                // There is only one non taken branch, no need to add submenus
                // But we need to modify the Solve Formula menu with more info and the path constraint index
                // The tooltip is not updated on the update event, we need to unregister the Solve formula submenu and add a new one
                unsigned int path_constraint_index = 0;
                for (const auto& pc : api.getPathConstraints()) {
                    for (auto const& [taken, srcAddr, dstAddr, pc] : pc.getBranchConstraints()) {
                        if (cur_ea == srcAddr && !taken) { // get the non taken branch for the path constraint the user clicked on          
                            // Using the solve formula as template
                            action_desc_t action = action_IDA_solve_formula_sub;
                            action.name = "Ponce:solve_formula_one_branch";

                            
                            char label[256];
                            qsnprintf(label, sizeof(label), "Solve formula to take " MEM_FORMAT, dstAddr);
                            action.label = label;

                            //We need the path constraint index during the action activate
                            char tooltip[256];
                            qsnprintf(tooltip, 255, "%s. Index: %u", action_IDA_solve_formula_sub.tooltip, path_constraint_index);
                            action.tooltip = tooltip;

                            bool success = register_action(action);
                            //If the submenu is already registered, we should unregister it and re-register it
                            if (!success) {
                                unregister_action(action.name);
                                success = register_action(action);
                            }
                            assert(success);
                            success = attach_action_to_popup(form, popup_handle, action.name, "SMT Solver/Solve formula", SETMENU_INS);
                            assert(success);
                            break;
                        }
                    }
                    path_constraint_index++;
                }
            }
            else {
                // There are more than one non taken branches, we add submenus
                unsigned int path_constraint_index = 0;
                for (const auto& pc : api.getPathConstraints()) {
                    for (auto const& [taken, srcAddr, dstAddr, pc] : pc.getBranchConstraints()) {
                        if (cur_ea == srcAddr && taken) { // get the taken branch for the path constraint the user clicked on          
                            // Using the solve formula as template (If not we modify the name of the main solve formula menu)
                            action_desc_t action = action_IDA_solve_formula_sub;
                            char name[256];
                            qsnprintf(name, 255, "[%u]Ponce:solve_formula_sub", path_constraint_index);
                            action.name = name;
                            //We need the path constrain index during the action activate
                            char tooltip[256];
                            qsnprintf(tooltip, 255, "%s. Index: %u", action_IDA_solve_formula_sub.tooltip, path_constraint_index);
                            action.tooltip = tooltip;
                            char label[256];
                            qsnprintf(label, 255, "Hit %u, taken -> " MEM_FORMAT, path_constraint_index, dstAddr);
                            action.label = label;

                            bool success = register_action(action);
                            //If the submenu is already registered, we should unregister it and re-register it
                            if (!success) {
                                unregister_action(action.name);
                                success = register_action(action);
                            }
                            assert(success);
                            success = attach_action_to_popup(form, popup_handle, action.name, "SMT Solver/Solve formula (Multiple hits)/", SETMENU_INS);
                            assert(success);
                        }
                    }
                    path_constraint_index++;
                }
            }
        }

        break;
    }
    case dbg_process_exit:
    {
        unhook_from_notification_point(HT_DBG, ui_callback, NULL);
        break;
    }
    }
    return 0;
}


