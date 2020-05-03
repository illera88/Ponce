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
// Ponce
#include "callbacks.hpp"
#include "globals.hpp"
#include "context.hpp"
#include "utils.hpp"
#include "tainting_n_symbolic.hpp"
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
    if (cmdOptions.showExtraDebugInfo)
        msg("[+] Notification code: %d str: %s\n", notification_code, notification_code_to_string(notification_code).c_str());
    switch (notification_code)
    {
    case dbg_process_start:
    case dbg_process_attach:
    {
        if (cmdOptions.showDebugInfo)
            msg("[+] Starting the debugged process. Reseting all the engines.\n");
        triton_restart_engines();
        clear_requests_queue();
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
        if (ponce_runtime_status.last_triton_instruction->getDisassembly().find("call dword ptr fs:[0xc0]") != -1)
        {
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
        if (cmdOptions.limitInstructionsTracingMode && ponce_runtime_status.current_trace_counter >= cmdOptions.limitInstructionsTracingMode)
        {
            int answer = ask_yn(1, "[?] %u instructions has been traced. Do you want to execute %u more?", ponce_runtime_status.total_number_traced_ins, (unsigned int)cmdOptions.limitInstructionsTracingMode);
            if (answer == 0 || answer == -1) //No or Cancel
            {
                // stop the trace mode and suspend the process
                disable_step_trace();
                suspend_process();
                msg("[!] Process suspended (Traced %d instructions)\n", ponce_runtime_status.total_number_traced_ins);
            }
            else
            {
                ponce_runtime_status.current_trace_counter = 0;
            }
        }

        //Check if the time limit for tracing was reached
        if (cmdOptions.limitTime != 0)
        {
            //This is the first time we start the tracer
            if (ponce_runtime_status.tracing_start_time == 0)
            {
                ponce_runtime_status.tracing_start_time = GetTimeMs64();
            }
            else if ((GetTimeMs64() - ponce_runtime_status.tracing_start_time) / 1000 >= cmdOptions.limitTime)
            {
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
        ponce_runtime_status.is_ponce_tracing_enabled = false;
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
        TWidget* form = va_arg(va, TWidget*);
        TPopupMenu* popup_handle = va_arg(va, TPopupMenu*);
        int view_type = get_widget_type(form);

        //Adding a separator
        attach_action_to_popup(form, popup_handle, "", SETMENU_INS);

        /*Iterate over all the actions*/
        for (int i = 0;; i++)
        {
            if (action_list[i].action_decs == NULL)
                break;

            /*Iterate over the view types of every action*/
            for (int j = 0;; j++)
            {
                if (action_list[i].view_type[j] == __END__) {
                    break;
                }
                if (action_list[i].view_type[j] == view_type)
                {
                    //We only attach to the popup if the action makes sense with the current configuration
                    if (cmdOptions.use_tainting_engine && action_list[i].enable_taint || cmdOptions.use_symbolic_engine && action_list[i].enable_symbolic)
                    {
                        attach_action_to_popup(form, popup_handle, action_list[i].action_decs->name, action_list[i].menu_path, SETMENU_INS);
                    }
                }
            }
        }       

        //Adding a separator
        attach_action_to_popup(form, popup_handle, "", SETMENU_INS);

        break;
    }
    case ui_finish_populating_widget_popup:
    {
        //This event is call after all the Ponce menus have been added and updated
        //It is the perfect point to add the multiple condition solve submenus
        TWidget* form = va_arg(va, TWidget*);
        TPopupMenu* popup_handle = va_arg(va, TPopupMenu*);
        int view_type = get_widget_type(form);

        //We get the ea form a global variable that is set in the update event
        //This is not very elegant but I don't know how to do it from here
        ea_t cur_ea = popup_menu_ea;
        // Don't let solve formulas if user is debugging natively
        if (view_type == BWN_DISASM && !(is_debugger_on() && !ponce_runtime_status.runtimeTrigger.getState())) { 
            std::set<triton::uint64> symbolic_adresses;
            unsigned int bound = 0;
            for (const auto& pc : api.getPathConstraints()) {
                if (pc.getTakenPredicate()->isSymbolized() && pc.isMultipleBranches()) {
                    for (auto const& [taken, srcAddr, dstAddr, pc] : pc.getBranchConstraints()) {
                        if (taken) {
                            // If it's the taken branch
                            if (cur_ea == srcAddr) { // if we right click on a symbolic branch                               
                                char name[256];
                                //We put the index at the beginning that we will use to get the bound from the action_name
                                qsnprintf(name, 255, "[%u]_Ponce:solve_formula_sub", bound);
                                action_IDA_solve_formula_sub.name = name;
                                char label[256];
                                qsnprintf(label, 255, "[%u]. " MEM_FORMAT " -> " MEM_FORMAT, bound, srcAddr, dstAddr);

                                action_IDA_solve_formula_sub.label = label;
                                bool success = register_action(action_IDA_solve_formula_sub);
                                //If the submenu is already registered, we should unregister it and re-register it
                                if (!success) {
                                    unregister_action(action_IDA_solve_formula_sub.name);
                                    success = register_action(action_IDA_solve_formula_sub);
                                }
                                success = attach_action_to_popup(form, popup_handle, action_IDA_solve_formula_sub.name, "SMT/Solve formula/", SETMENU_INS);
                                break;
                            }
                        }
                    }
                    bound++;
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


