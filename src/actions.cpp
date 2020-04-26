//! \file
/*
**  Copyright (c) 2016 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

//IDA
#include <idp.hpp>
#include <dbg.hpp>
#include <loader.hpp>
#include <kernwin.hpp>

//Ponce
#include "globals.hpp"
#include "utils.hpp"
#include "callbacks.hpp"
#include "formConfiguration.hpp"
#include "formTaintSymbolizeInput.hpp"
#include "actions.hpp"
#include "formTaintWindow.hpp"
#include "blacklist.hpp"
#include "context.hpp"
#include "solver.hpp"
#include "triton_logic.hpp"

//Triton
#include "triton/api.hpp"
#include "triton/x86Specifications.hpp"


int symbolize_register(qstring selected, action_activation_ctx_t* action_activation_ctx) {
    const triton::arch::Register* register_to_symbolize = str_to_register(selected);

    if (register_to_symbolize)
    {
        /*When the user symbolize something for the first time we should enable step_tracing*/
        start_tainting_or_symbolic_analysis();

        msg("[!] Symbolizing register %s\n", selected.c_str());
        char comment[256];
        ea_t pc = action_activation_ctx->cur_ea;
        qsnprintf(comment, 256, "Reg %s at address: " MEM_FORMAT, selected.c_str(), pc);

        // Before symbolizing register we should set his concrete value
        needConcreteRegisterValue_cb(api, *register_to_symbolize);

        // Symbolize register
        api.symbolizeRegister(*register_to_symbolize, std::string(comment));

        //If last_instruction is not set this instruction is not analyze
        if (ponce_runtime_status.last_triton_instruction == NULL)
        {
            tritonize(pc);
            return 0;
        }
        //If the register symbolize is a source for the instruction then we need to reanalize the instruction
        //So the self instruction will be tainted
        auto read_registers = ponce_runtime_status.last_triton_instruction->getReadRegisters();
        for (auto it = read_registers.begin(); it != read_registers.end(); it++)
        {
            auto reg = it->first;
            if (reg.getId() == register_to_symbolize->getId())
            {
                tritonize(pc);
                break;
            }
        }
    }
    return 0;
}


struct ah_taint_register_t : public action_handler_t
{
    /*Event called when the user taint a register*/
    virtual int idaapi activate(action_activation_ctx_t*)
    {
        // Get the address range selected, or return false if there was no selection
        bool res = false;
        qstring selected;
        uint32 flags;
        res = get_highlight(&selected, get_current_viewer(), &flags);

        if (res)
        {
            const triton::arch::Register* register_to_taint = str_to_register(selected);

            if (register_to_taint)
            {
                msg("[!] Tainting register %s\n", selected.c_str());
                api.taintRegister(*register_to_taint);

                //When the user taints something for the first time we should enable step_tracing
                start_tainting_or_symbolic_analysis();
                //If last_instruction is not set this instruction is not analyze
                if (ponce_runtime_status.last_triton_instruction == NULL)
                {
                    tritonize(current_instruction());
                    return 0;
                }
                //If the register tainted is a source for the instruction then we need to reanalize the instruction
                //So the self instruction will be tainted
                auto read_registers = ponce_runtime_status.last_triton_instruction->getReadRegisters();
                for (auto it = read_registers.begin(); it != read_registers.end(); it++)
                {
                    auto reg = it->first;
                    //msg("Register read: %s\n", reg.getName().c_str());
                    if (reg.getId() == register_to_taint->getId())
                    {
                        tritonize(current_instruction());
                        break;
                    }
                }
            }
        }
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* action_update_ctx)
    {
        //Only if process is being debugged
        if (is_debugger_on())
        {
            qstring selected;
            uint32 flags;
            if (get_highlight(&selected, get_current_viewer(), &flags))
            {
                if (str_to_register(selected))
                    return AST_ENABLE;
            }
        }
        return AST_DISABLE;
    }
};
static ah_taint_register_t ah_taint_register;

static const action_desc_t action_IDA_taint_register = ACTION_DESC_LITERAL(
    "Ponce:taint_register", // The action name. This acts like an ID and must be unique
    "Taint Register", //The action text.
    &ah_taint_register, //The action handler.
    "Ctrl+Shift+R", //Optional: the action shortcut
    "Taint the selected register", //Optional: the action tooltip (available in menus/toolbar)
    50); //Optional: the action icon (shows when in menus/toolbars)

struct ah_symbolize_register_t : public action_handler_t
{
    /*Event called when the user symbolize a register*/
    virtual int idaapi activate(action_activation_ctx_t* action_activation_ctx)
    {
        bool res = false;
        // Get the address range selected, or return false if there was no selection
        qstring selected;
        uint32 flags;
        res = get_highlight(&selected, get_current_viewer(), &flags);

        if (res)
        {
            symbolize_register(selected, action_activation_ctx);
        }
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* action_update_ctx_t)
    {
        //Only if process is being debugged
        if (is_debugger_on())
        {
            qstring selected;
            uint32 flags;
            if (get_highlight(&selected, get_current_viewer(), &flags))
            {
                if (str_to_register(selected))
                    return AST_ENABLE;
            }
        }
        return AST_DISABLE;
    }
};
static ah_symbolize_register_t ah_symbolize_register;

static const action_desc_t action_IDA_symbolize_register = ACTION_DESC_LITERAL(
    "Ponce:symbolize_register", // The action name. This acts like an ID and must be unique
    "Symbolize Register", //The action text.
    &ah_symbolize_register, //The action handler.
    "Ctrl+Shift+R", //Optional: the action shortcut
    "Symbolize the selected register", //Optional: the action tooltip (available in menus/toolbar)
    50); //Optional: the action icon (shows when in menus/toolbars)

struct ah_taint_memory_t : public action_handler_t
{
    /*Event called when the user taint a memory*/
    virtual int idaapi activate(action_activation_ctx_t* action_activation_ctx)
    {
        ea_t selection_starts = 0;
        ea_t selection_ends = 0;
        //In the dissas windows we use the whole item selected. If we have a string we can't select only some bytes from the dissas windows
        if (action_activation_ctx->widget_type == BWN_DISASM)
        {
            //We don't know how to do this :( 
            //msg("[!] Disassembly windows no supported for memory tainting...yet\n");
            //So we ask to the user for the memory and the size
            // this is just a hint for the user. they can change the filled value
            if (!prompt_window_taint_symbolize(get_screen_ea(), &selection_starts, &selection_ends))
                return 0;
        }
        else
            return 0;
        // ToDo: give another thought about variable lenght symbolic arguments.
        //The selection ends in the last item, we need to add 1 to calculate the length
        ea_t selection_length = selection_ends - selection_starts;
        msg("[+] Tainting memory from " MEM_FORMAT " to " MEM_FORMAT ". Total: %d bytes\n", selection_starts, selection_ends, (int)selection_length);

        //Tainting all the selected memory
        api.taintMemory(triton::arch::MemoryAccess(selection_starts, selection_length));
        /*When the user taints something for the first time we should enable step_tracing*/
        start_tainting_or_symbolic_analysis();
        //If last_instruction is not set this instruction is not analyze
        if (ponce_runtime_status.last_triton_instruction == NULL)
        {
            tritonize(current_instruction());
            return 0;
        }
        //We reanalyse the instruction where the pc is right now
        auto store_access_list = ponce_runtime_status.last_triton_instruction->getStoreAccess();
        for (auto it = store_access_list.begin(); it != store_access_list.end(); it++)
        {
            triton::arch::MemoryAccess memory_access = it->first;
            //If the address is inside the range just tainted, then reanalize
            if (memory_access.getAddress() >= selection_starts && memory_access.getAddress() < selection_starts + selection_length)
            {
                tritonize(current_instruction());
                break;
            }
        }
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* action_update_ctx_t)
    {
        return AST_DISABLE;
    }
};
static ah_taint_memory_t ah_taint_memory;

static const action_desc_t action_IDA_taint_memory = ACTION_DESC_LITERAL(
    "Ponce:taint_memory", // The action name. This acts like an ID and must be unique
    "Taint Memory", //The action text.
    &ah_taint_memory, //The action handler.
    "Ctrl+Shift+M", //Optional: the action shortcut
    "Taint the selected register", //Optional: the action tooltip (available in menus/toolbar)
    50); //Optional: the action icon (shows when in menus/toolbars)

struct ah_symbolize_memory_t : public action_handler_t
{
    /*Event called when the user symbolize a memory*/
    virtual int idaapi activate(action_activation_ctx_t* action_activation_ctx)
    {
        ea_t selection_starts = 0;
        ea_t selection_ends = 0;
        //In the dissas windows we use the whole item selected. If we have a string we can't select only some bytes from the dissas windows
        if (action_activation_ctx->widget_type == BWN_DISASM)
        {
            //We ask to the user for the memory and the size
            if (!prompt_window_taint_symbolize(get_screen_ea(), &selection_starts, &selection_ends))
                return 0;
        }
        else
            return 0;

        /* When the user taints something for the first time we should enable step_tracing*/
        start_tainting_or_symbolic_analysis();

        //The selection ends in the last item which is included, so we need to add 1 to calculate the length
        auto selection_length = selection_ends - selection_starts;
        msg("[+] Symbolizing memory from " MEM_FORMAT " to " MEM_FORMAT ". Total: %d bytes\n", selection_starts, selection_ends, (int)selection_length);

        // Before symbolizing the memory we should set its concrete value
        for (unsigned int i = 0; i < selection_length; i++) {
            needConcreteMemoryValue_cb(api, triton::arch::MemoryAccess(selection_starts + i, 1));
        }

        // Symbolizing all the selected memory
        symbolize_all_memory(selection_starts, selection_length);

        tritonize(current_instruction());
        return 0;

        //If last_instruction is not set this instruction is not analyze
        if (ponce_runtime_status.last_triton_instruction == nullptr)
        {
            // ToDo: What happens if Triton is enable when at a blacklisted function (printf, fgets...)?
            // It wont be blacklisted. We should change t
            tritonize(current_instruction());
            return 0;
        }
        //We reanalyse the instruction where the pc is right now if the instruction was reading the memory that was just symbolized
        auto load_access_list = ponce_runtime_status.last_triton_instruction->getLoadAccess();
        for (auto it = load_access_list.begin(); it != load_access_list.end(); it++)
        {
            triton::arch::MemoryAccess memory_access = it->first;
            //If the address is inside the range just symbolized, then reanalize
            if (memory_access.getAddress() >= selection_starts && memory_access.getAddress() < selection_starts + selection_length)
            {
                tritonize(current_instruction());
                break;
            }
        }
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* action_update_ctx_t)
    {
        //Only if process is being debugged
        if (is_debugger_on())
        {
            if (action_update_ctx_t->widget_type == BWN_DUMP)
            {
                if (action_update_ctx_t->cur_sel.from.at != NULL && action_update_ctx_t->cur_sel.to.at != NULL)
                {
                    auto selection_starts = action_update_ctx_t->cur_sel.from.at->toea();
                    auto selection_ends = action_update_ctx_t->cur_sel.to.at->toea();
                    int diff = (int)(selection_ends - selection_starts);
                    if (diff >= 0)
                        return AST_ENABLE;
                }
            }
            else
            {
                return AST_ENABLE;
            }
        }
        return AST_DISABLE;
    }
};
static ah_symbolize_memory_t ah_symbolize_memory;

static const action_desc_t action_IDA_symbolize_memory = ACTION_DESC_LITERAL(
    "Ponce:symbolize_memory", // The action name. This acts like an ID and must be unique
    "Symbolize Memory", //The action text.
    &ah_symbolize_memory, //The action handler.
    "Ctrl+Shift+M", //Optional: the action shortcut
    "Symbolize the selected register", //Optional: the action tooltip (available in menus/toolbar)
    50); //Optional: the action icon (shows when in menus/toolbars)

struct ah_negate_and_inject_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* action_activation_ctx)
    {
        //This is only working from the disassembly windows
        if (action_activation_ctx->widget_type == BWN_DISASM)
        {
            ea_t pc = action_activation_ctx->cur_ea;
            msg("[+] Negating condition at " MEM_FORMAT "\n", pc);

            //We want to negate the last path contraint at the current address, so we use as a bound the size of the path constrains
            unsigned int bound = ponce_runtime_status.myPathConstraints.size() - 1;
            auto solutions = solve_formula(pc, bound);
            //if (input_ptr != NULL)
            //{
            //    //We need to modify the last path constrain
            //    auto temp = ponce_runtime_status.myPathConstraints[bound]->notTakenAddr;
            //    ponce_runtime_status.myPathConstraints[bound]->notTakenAddr = ponce_runtime_status.myPathConstraints[bound]->takenAddr;
            //    ponce_runtime_status.myPathConstraints[bound]->takenAddr = temp;
            //    //We need to modify the condition flags to negate the condition
            //    if (ponce_runtime_status.last_triton_instruction->getAddress() == pc)
            //    {
            //        negate_flag_condition(ponce_runtime_status.last_triton_instruction);
            //    }
            //    // We set the results obtained from solve_formula
            //    set_SMT_results(input_ptr);

            //    //delete it after setting the proper results
            //    delete input_ptr;
            //}
        }
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* action_update_ctx_t)
    {
        //Only if process is being debugged
        if (is_debugger_on())
        {
            //If we are in runtime and it is the last instruction we test if it is symbolize
            if (ponce_runtime_status.last_triton_instruction != NULL && ponce_runtime_status.last_triton_instruction->getAddress() == action_update_ctx_t->cur_ea && ponce_runtime_status.last_triton_instruction->isBranch() && ponce_runtime_status.last_triton_instruction->isSymbolized())
                return AST_ENABLE;
        }
        return AST_DISABLE;
    }
};
static ah_negate_and_inject_t ah_negate_and_inject;

static const action_desc_t action_IDA_negate = ACTION_DESC_LITERAL(
    "Ponce:negate_and_inject", // The action name. This acts like an ID and must be unique
    "Negate & Inject", //The action text.
    &ah_negate_and_inject, //The action handler.
    "Ctrl+Shift+N", //Optional: the action shortcut
    "Negate the current condition and inject the solution into memory", //Optional: the action tooltip (available in menus/toolbar)
    58); //Optional: the action icon (shows when in menus/toolbars)

struct ah_negate_inject_and_restore_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* action_activation_ctx)
    {
        //This is only working from the disassembly windows
        if (action_activation_ctx->widget_type == BWN_DISASM)
        {
            ea_t pc = action_activation_ctx->cur_ea;
            msg("[+] Negating condition at " MEM_FORMAT "\n", pc);

            //We need to get the instruction associated with this address, we look for the addres in the map
            //We want to negate the last path contraint at the current address, so we traverse the myPathconstraints in reverse

            unsigned int bound = ponce_runtime_status.myPathConstraints.size() - 1;
            auto solutions = solve_formula(pc, bound);
            //if (solutions != NULL)
            //{
            //    //Restore the snapshot
            //    snapshot.restoreSnapshot();

            //    // We set the results obtained from solve_formula
            //    set_SMT_results(input_ptr);

            //    //delete it after setting the proper results
            //    delete input_ptr;
            //}
        }
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* action_update_ctx_t)
    {
        //Only if process is being debugged
        if (is_debugger_on() && snapshot.exists())
        {
            //If we are in runtime and it is the last instruction we test if it is symbolize
            if (ponce_runtime_status.last_triton_instruction != NULL && ponce_runtime_status.last_triton_instruction->getAddress() == action_update_ctx_t->cur_ea && ponce_runtime_status.last_triton_instruction->isBranch() && ponce_runtime_status.last_triton_instruction->isSymbolized())
                return AST_ENABLE;
        }
        return AST_DISABLE;
    }
};
static ah_negate_inject_and_restore_t ah_negate_inject_and_restore;

static const action_desc_t action_IDA_negateInjectRestore = ACTION_DESC_LITERAL(
    "Ponce:negate_inject_restore", // The action name. This acts like an ID and must be unique
    "Negate, Inject & Restore snapshot", //The action text.
    &ah_negate_inject_and_restore, //The action handler.
    "Ctrl+Shift+I", //Optional: the action shortcut
    "Negates a condition, inject the solution and restore the snapshot", //Optional: the action tooltip (available in menus/toolbar)
    145); //Optional: the action icon (shows when in menus/toolbars)

struct ah_create_snapshot_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx)
    {
        ea_t xip;
        if (!get_ip_val(&xip)) {
            msg("Could not get the XIP value. This should never happen\n");
            return 0;
        }
        ponce_set_cmt(xip, "Snapshot taken here", false);
        snapshot.takeSnapshot();
        snapshot.setAddress(xip); // We will use this address later to delete the comment
        msg("Snapshot Taken\n");
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* ctx)
    {
        //Only if process is being debugged and there is not previous snaphot
        if (is_debugger_on() && !snapshot.exists())
            return AST_ENABLE;
        else
            return AST_DISABLE;
    }
};
static ah_create_snapshot_t ah_create_snapshot;

static const action_desc_t action_IDA_createSnapshot = ACTION_DESC_LITERAL(
    "Ponce:create_snapshot",
    "Create Execution Snapshot",
    &ah_create_snapshot,
    "Ctrl+Shift+C",
    NULL,
    129);

struct ah_restore_snapshot_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx)
    {
        snapshot.restoreSnapshot();
        msg("Snapshot restored\n");
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* ctx)
    {
        //Only if process is being debugged and there is an existent shapshot
        if (is_debugger_on() && snapshot.exists())
            return AST_ENABLE;
        else
            return AST_DISABLE;
    }
};
static ah_restore_snapshot_t ah_restore_snapshot;

static const action_desc_t action_IDA_restoreSnapshot = ACTION_DESC_LITERAL(
    "Ponce:restore_snapshot",
    "Restore Execution Snapshot",
    &ah_restore_snapshot,
    "Ctrl+Shift+S",
    NULL,
    128);

struct ah_delete_snapshot_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx)
    {
        snapshot.resetEngine();
        msg("[+] Snapshot removed\n");
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* ctx)
    {
        //Only if there is an existent shapshot
        if (snapshot.exists())
            return AST_ENABLE;
        else
            return AST_DISABLE;
    }
};
static ah_delete_snapshot_t ah_delete_snapshot;

static const action_desc_t action_IDA_deleteSnapshot = ACTION_DESC_LITERAL(
    "Ponce:delete_snapshot",
    "Delete Execution Snapshot",
    &ah_delete_snapshot,
    "Ctrl+Shift+D",
    NULL,
    130);

struct ah_show_config_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx)
    {
        prompt_conf_window();
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* ctx)
    {
        return AST_ENABLE_ALWAYS;
    }
};
static ah_show_config_t ah_show_config;

action_desc_t action_IDA_show_config = ACTION_DESC_LITERAL(
    "Ponce:show_config", // The action name. This acts like an ID and must be unique
    "Show config", //The action text.
    &ah_show_config, //The action handler.
    "Ctrl+Shift+P", //Optional: the action shortcut
    "Show the Ponce configuration", //Optional: the action tooltip (available in menus/toolbar)
    156); //Optional: the action icon (shows when in menus/toolbars)

struct ah_show_taintWindow_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx)
    {
        //So we don't reopen twice the same window
        auto form = find_widget("Taint Window");
        if (form != NULL) {
            //let's update it and change to it
            fill_entryList();
            activate_widget(form, true);
        }

        else
            create_taint_window();

        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* ctx)
    {
        return AST_ENABLE_ALWAYS;
    }
};
static ah_show_taintWindow_t ah_show_taintWindow;

action_desc_t action_IDA_show_taintWindow = ACTION_DESC_LITERAL(
    "Ponce:show_taintWindows", // The action name. This acts like an ID and must be unique
    "Show Taint/Symbolic items", //The action text.
    &ah_show_taintWindow, //The action handler.
    "Alt+Shift+T", //Optional: the action shortcut
    "Show all the taint or symbolic items", //Optional: the action tooltip (available in menus/toolbar)
    157); //Optional: the action icon (shows when in menus/toolbars)

struct ah_unload_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx)
    {
        term();
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* ctx)
    {
        return AST_ENABLE_ALWAYS;
    }
};
static ah_unload_t ah_unload;

action_desc_t action_IDA_unload = ACTION_DESC_LITERAL(
    "Ponce:unload", // The action name. This acts like an ID and must be unique
    "Unload plugin", //The action text.
    &ah_unload, //The action handler.
    "Ctrl+Shift+U", //Optional: the action shortcut
    "Unload the plugin", //Optional: the action tooltip (available in menus/toolbar)
    138); //Optional: the action icon (shows when in menus/toolbars)

struct ah_execute_native_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx)
    {
        if (ask_for_execute_native())
        {
            //Deleting previous snapshot
            snapshot.resetEngine();
            //Disabling step tracing...
            disable_step_trace();
            //And continue! (F9)
            continue_process();
        }
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* ctx)
    {
        //Only if process is being debugged
        if (is_debugger_on())
        {
            return AST_ENABLE;
        }
        return AST_DISABLE;
    }
};
static ah_execute_native_t ah_execute_native;

action_desc_t action_IDA_execute_native = ACTION_DESC_LITERAL(
    "Ponce:execute_native", // The action name. This acts like an ID and must be unique
    "Execute native", //The action text.
    &ah_execute_native, //The action handler.
    "Ctrl+Shift+F9", //Optional: the action shortcut
    "Execute native without tracing every instruction until next breakpoint", //Optional: the action tooltip (available in menus/toolbar)
    113); //Optional: the action icon (shows when in menus/toolbars)

struct ah_enable_disable_tracing_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx)
    {
        if (ponce_runtime_status.runtimeTrigger.getState())
        {
            if (ask_for_execute_native())
            {
                //Deleting previous snapshot
                snapshot.resetEngine();
                //Disabling step tracing...
                disable_step_trace();
                ponce_runtime_status.runtimeTrigger.disable();
                if (cmdOptions.showDebugInfo)
                    msg("Disabling step tracing\n");
            }
        }
        else
        {
            start_tainting_or_symbolic_analysis();
            tritonize(current_instruction());
            if (cmdOptions.showDebugInfo)
                msg("[+] Enabling step tracing\n");
        }
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* ctx)
    {
        //Only if process is being debugged
        if (is_debugger_on())
        {
            //We are using this event to change the text of the action
            if (ponce_runtime_status.runtimeTrigger.getState())
            {
                update_action_label(ctx->action, "Disable ponce tracing");
                update_action_icon(ctx->action, 62);
            }
            else
            {
                update_action_label(ctx->action, "Enable ponce tracing");
                update_action_icon(ctx->action, 61);
            }

            //So...we use this update event to update the ea, and then in the 
            //ui_finish_populating_tform_popup to add the sunmenus for solve
            popup_menu_ea = ctx->cur_ea;
            return AST_ENABLE;
        }
        return AST_DISABLE;
    }
};
static ah_enable_disable_tracing_t ah_enable_disable_tracing;

//We need to define this struct before the action handler because we are using it inside the handler
action_desc_t action_IDA_enable_disable_tracing = ACTION_DESC_LITERAL(
    "Ponce:enable_disable_tracing",
    "Enable ponce tracing", //The action text.
    &ah_enable_disable_tracing, //The action handler.
    "Ctrl+Shift+E", //Optional: the action shortcut
    "Enable or Disable the ponce tracing", //Optional: the action tooltip (available in menus/toolbar)
    61); //Optional: the action icon (shows when in menus/toolbars)


struct ah_solve_formula_sub_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx)
    {
        //We extract the solved index from the action name
        unsigned int condition_index = atoi((ctx->action+1)); // sikp [ in action name
        if (cmdOptions.showDebugInfo)
            msg("[+] Solving condition at address " MEM_FORMAT " with bound %d\n", ctx->cur_ea, condition_index);
        auto solutions = solve_formula(ctx->cur_ea, condition_index);
        if (!solutions.empty()) {
            //print info
        }
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* ctx)
    {
        return AST_ENABLE;
    }
};
ah_solve_formula_sub_t ah_solve_formula_sub;

action_desc_t action_IDA_solve_formula_sub = ACTION_DESC_LITERAL(
    "Ponce:solve_formula_sub", // The action name. This acts like an ID and must be unique
    "", //The action text.
    &ah_solve_formula_sub, //The action handler.
    "", //Optional: the action shortcut
    "The solves a specific conditions and shows the result in the output window", //Optional: the action tooltip (available in menus/toolbar)
    13); //Optional: the action icon (shows when in menus/toolbars)


struct ah_ponce_banner_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx)
    {
        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* ctx)
    {
        unsigned char ponce_icon[] = "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A\x00\x00\x00\x0D\x49\x48\x44\x52\x00\x00\x00\x20\x00\x00\x00\x20\x08\x06\x00\x00\x00\x73\x7A\x7A\xF4\x00\x00\x00\x01\x73\x52\x47\x42\x00\xAE\xCE\x1C\xE9\x00\x00\x00\x04\x67\x41\x4D\x41\x00\x00\xB1\x8F\x0B\xFC\x61\x05\x00\x00\x00\x09\x70\x48\x59\x73\x00\x00\x0E\xC4\x00\x00\x0E\xC4\x01\x95\x2B\x0E\x1B\x00\x00\x0C\xC4\x49\x44\x41\x54\x58\x47\x45\x97\x77\x54\x94\x69\x9A\xC5\x6B\xF7\x9C\xDD\xB3\x33\x67\x66\xDA\x2C\x92\xA1\x08\x05\x14\x59\x42\x21\x85\x04\x11\x49\x22\x2A\x51\x25\x08\x88\x0D\x8A\x28\x60\x00\xEC\x56\x51\x14\x23\x88\x82\xA0\x24\xC5\x00\x82\x80\x18\xDA\x00\x54\x51\x44\xC1\xD0\x3D\x1D\xB4\x1D\xDB\x9E\x99\xD3\xDB\x7B\xBA\x7B\x66\xCE\x9E\x8D\x67\xFE\xF9\xED\x53\x65\xDB\xFB\xC7\x3D\xDF\x57\x1F\x1F\x75\xEF\x7B\x9F\xE7\xBD\xEF\x53\x8A\x3A\xDD\xD7\xD4\x8D\x7C\xCD\xE9\xA1\xAF\x38\x33\xFC\x92\xDA\x91\x57\x72\xFF\xA5\x3C\x7B\xC5\xF9\xC9\x3F\x73\x4A\xFF\x86\xFD\x1D\x83\xEC\xD9\xB7\x8F\xAA\x82\x4D\xB4\xEC\xDB\x42\xAB\xA0\xAD\x3C\x9F\x96\xBD\x79\x9C\x2B\xC9\xA6\x66\x5B\x1A\x95\x39\xEB\x29\xCB\x4C\x64\x67\xC6\x3A\x2A\x2E\xF6\xD2\x30\xFE\x2D\xF5\xC3\x5F\x71\x4E\x27\xDF\xA3\xFF\xDA\x74\xAD\x1F\x79\xF9\xCB\x67\xE3\xFD\x59\x81\xC2\x48\x58\x27\x0F\x6B\xE5\xE5\x3A\xA3\x00\x11\x72\x61\xF2\x4F\x9C\xD6\xBD\xA6\xB2\xED\x36\xFB\x76\x97\x71\xFC\xC3\x54\x5A\xCB\xB2\xB8\x5C\x91\x6F\x22\x37\x12\x1B\x05\xBC\x17\xD2\x54\x96\x43\xED\x8E\x8D\x1C\xD8\x92\xC4\xB6\xA4\x28\x0A\xD2\x13\x38\x7E\x7B\x9A\x73\xFA\xD7\xD4\x3D\xFE\x82\xB3\xC3\x5F\x9A\xC4\x18\x51\x27\x8B\x3B\x6B\xBC\x17\x9E\x73\x46\x01\x46\x15\x26\xC8\x1F\xCE\x89\x1B\x17\xC6\xDF\x52\x5C\x7F\x83\xBC\x82\x02\x8A\x92\x57\x72\x7C\x4B\x22\x2D\xBB\x73\x68\xFD\x99\xF0\x3D\xE9\x7B\x21\xEF\x90\x2B\x22\x32\x39\x5B\xB4\x91\xC3\xF9\x29\x94\xA7\x47\x51\xB2\xAF\x82\x46\x71\xF0\x82\xE1\x1B\x9A\x26\xBE\xE5\xC2\x84\xF1\xFA\x96\xA6\xF1\x37\x72\x2F\xD0\xBD\xA4\x41\xF7\x25\x8A\xF7\xD6\x34\x18\xDE\x88\xBA\x2F\x88\xD9\x90\x8B\xD9\xA2\x85\xC4\x2F\xF7\x67\xB7\x58\x7A\xA2\x30\x9D\xA6\xD2\x6C\x93\xF5\xEF\x09\x2F\xED\x31\x22\x97\x36\x79\xD6\x21\xAE\xB4\x95\xE7\xD2\xB2\x27\x8B\x3A\x71\xA1\xA6\x20\x95\xED\xC9\xAB\xC8\x4C\x8C\xE2\x60\xCB\x4D\x8E\xB4\xDE\xE4\x70\xD3\x55\x8E\xB6\x76\x51\x71\xAE\x9D\x9D\x27\x9B\x39\xD8\x3E\xC0\xB9\xA1\xCF\x68\x99\xFA\x46\x04\x0C\x7F\x41\xE3\xE8\x2B\x6A\xFA\x27\xF0\x5F\x1E\xC1\xC2\x79\x73\xB0\xB6\xB6\xC1\xD5\xCD\x85\x1D\xE9\xF1\xD4\x6E\x4F\xA7\x71\x57\x26\xAD\x42\xFC\xCE\x81\x3C\xDA\x85\xB0\xAD\x3C\x87\x46\xA9\xFF\x89\xC2\x0D\x7C\x9C\xB3\x8E\xA2\xB4\x18\xB2\xE2\xC3\x49\x5A\x11\x44\x8C\x36\x80\x10\x3F\x6F\xB4\xBE\x1E\x84\xFB\x79\x10\xE6\xA7\x46\xEB\xE3\x46\x84\xAF\x2B\x81\xEE\x4E\x78\xB9\x3A\xE3\x17\x18\xC4\x8E\x93\x4D\x28\xEA\x1F\x3E\x17\x25\x6F\x49\x2A\x28\xC1\xCA\x6C\x3E\x16\x96\x96\x2C\x5C\xB4\x08\x17\x95\x23\x9B\x65\x15\xC6\xFA\x9F\x28\x4C\xA5\xAE\x78\xA3\x08\x31\xAE\x72\x13\xFB\x73\x12\xD9\x91\xB6\x8A\xDC\x35\x2B\x48\x89\xD2\x12\xAD\xF5\x43\xE3\xED\x8E\xD2\xDE\x0E\x4B\x2B\x2B\x59\x80\x35\xFE\x9E\xAE\xC4\x89\x8B\x61\x41\x4B\xD1\x88\x90\xE8\xE0\xA5\x04\xCA\x3B\x4E\x0E\xF6\xD8\xD9\x2B\xF9\xE0\xD7\xBF\x22\x3E\x6B\x2B\x8A\x06\xC3\xD7\xB4\x4E\xFF\x91\x8C\x9D\x15\x78\xAA\xEC\x59\x62\x66\xC6\xDC\xB9\x73\xB1\x92\x2F\x5A\x2E\xFF\xBC\x2B\x63\x0D\xE5\xD2\xE1\x25\x1B\xE2\xD9\x2A\x0D\xB6\x29\x36\x94\xF5\x11\x1A\x22\x35\x3E\xF8\xA8\x5D\x71\x72\x54\xCA\xBB\xD6\x38\x3B\x3A\xA0\x5D\xEA\x4E\xAC\x76\x29\x41\x42\xE8\xEF\xE3\x81\x8F\xA7\x1B\x3E\xEE\x6E\x78\xAA\x5D\x50\x3A\x38\x60\x2E\xEF\x2D\x59\xB2\x84\x79\xBF\xFB\x0D\xDA\xA8\x78\xCE\x3F\x7A\x8A\xA2\xB4\xA2\x92\xF3\xA3\x5F\xB1\xE5\xE3\xE3\xA8\x1D\x6D\x31\x33\x5B\xC2\x9C\x39\x73\x58\xB0\x60\x01\xF6\x4A\x3B\xD6\xAE\x0C\x21\x3B\x61\x85\x89\x38\x69\x65\x10\x11\x81\xDE\x78\xB8\x38\x61\x6B\x6B\x8B\xA3\x83\x12\xB5\x88\xF0\x70\x17\xB8\x09\x5C\x9C\xB1\xB7\xB5\x61\xF1\x12\x73\xEC\xEC\x6C\x70\x76\xB2\x47\xE5\xA4\xC4\xC1\xDE\x1E\x2B\x8B\x25\x2C\x5A\x30\x0F\x27\x95\x8A\x84\x8C\x2D\xD4\x7F\xF2\x84\xD6\xF1\x57\x28\xF6\xAD\x74\x61\x7F\xE5\x3E\x0A\x8E\x9C\x41\xED\xEC\x28\x0A\xCD\xF8\xC0\x28\x60\xE1\x22\xFC\xBD\x3D\x48\x8D\x0E\x25\x79\x65\x30\xA1\xFE\x5E\xB8\x38\x39\xE0\x2A\xEF\xA8\x1C\xED\x71\x50\x2A\xB1\x17\xCB\xCD\xCC\x2D\x04\xE6\xB8\xC8\xF3\xD8\xD0\x40\x4A\xB3\xD6\xD1\x71\xA8\x90\xD9\xCE\xA3\x18\xDA\x0F\xB3\x23\x7B\x35\x96\x36\xB6\x04\x48\x7F\xE5\x94\x1F\xA4\x76\x70\x94\xCB\x33\x7F\xA0\xD5\xF0\x39\x17\x75\x2F\x44\x40\xE2\x32\xAA\x37\xC7\x92\x97\xB6\x06\x57\x95\x13\x8B\x16\x2F\xC6\xC2\x7C\x89\xE9\xCB\xF2\xD6\x46\x4A\xED\xFC\xF1\x16\x0B\x7D\xA5\xA6\x4A\x5B\x6B\xDC\xE4\x1D\x07\x7B\x5B\xCC\x45\xA8\x9D\x08\xD8\x95\xB5\x9A\xBE\x33\x65\x7C\xDE\x73\x9A\xBF\xEA\x5B\xF9\xFB\x74\x17\x7F\x7F\x72\x93\xFF\x9C\xE8\xE2\x7F\x27\xBB\xE9\x39\x55\x4A\x55\xCB\x55\x3A\xA6\x5F\x73\x75\xF6\x0D\xAD\x63\x9F\xD3\x3C\xF2\x9C\x4B\xFA\x4F\xB9\x34\xFA\x19\x8A\xB8\x20\x6F\x76\xC9\x7E\x2F\xDD\x9C\x84\x9B\x94\xC0\xD3\x4D\x45\xD6\x6A\xE9\x66\x69\x2E\x3F\x2F\x35\x4B\x85\x38\xC0\x5B\x2D\x2B\xB6\x63\x91\x99\xF9\xBB\x15\x4B\x9F\x58\xDB\xDA\x71\x74\x77\x2E\x3F\x8E\x5C\xE2\xBF\xA7\x3A\xF9\xF7\xF1\xCB\xFC\x75\xB4\x83\x7F\x1B\x6A\xE5\x87\xE1\xCB\xFC\x4D\x7F\x85\xFF\x9A\xB8\x4A\xE7\xE1\x02\x4A\x8E\x1C\xA3\x63\xF2\x25\xCD\x43\xB3\xA6\x55\xB7\xEA\x9F\xD3\xA6\x97\xAB\xEE\x39\x8A\x98\x60\x1F\xE2\xFC\x5D\xA4\x8B\xD5\x44\x87\xF8\x89\xE5\xC1\xF8\x7B\xB9\xE2\x25\xAB\x0E\xD3\xF8\x8A\x00\x37\x53\xBD\x17\x2E\x32\xC3\xC3\xD9\x8A\x08\x3F\x3B\x1C\xA4\x04\x47\x64\x47\xFC\x20\xE4\x7F\x1B\x6D\xE3\xF5\x60\x3D\x2F\x07\xEA\x98\xBD\x7E\x9C\x9F\x74\x6D\xBC\xBD\xDB\x48\x7F\x5D\x19\x39\xA9\xB1\xAC\x8E\xD4\x50\x56\x52\xC0\xC5\x91\x67\xB4\xEB\x9F\x0A\xF1\x53\xDA\x47\xE5\x5E\xD0\x21\x50\x94\x94\xED\x22\x37\x7B\x13\xAB\x43\x7C\x09\x0E\xF0\x96\xC6\x92\xC6\x71\x76\x62\x79\xA0\x2F\x7E\xB2\x6D\x6C\x64\x4B\x59\x59\x5A\xB0\xC4\xC2\x92\xA4\x50\x47\x92\x42\xEC\xA4\xBB\x5D\x19\xBE\xB8\x9F\x4F\xBB\x6A\x28\xCD\x5D\xCB\x52\x79\x4F\x25\xFD\x61\x27\xCD\x76\xF7\xFC\x7E\xBE\x7F\xD8\xCC\x0E\xD9\xAA\xAE\x6E\x6E\xE4\xAD\x8B\x24\x3F\x2F\x53\x04\x3C\xA5\xC3\xF0\xDC\x84\xCB\x63\x2F\x7E\x81\xE2\x78\xC7\x0D\xB2\x32\xD2\x25\x1C\x9C\x70\x17\xFB\x97\x6B\xFC\x58\xA1\xF5\xC7\x41\x3A\xDC\xB8\x15\x9D\x1C\xAC\xB1\xB1\xB1\xC4\xDE\xCE\x9A\x60\x1F\x47\xC9\x07\xB9\xCA\x4E\x68\xAB\xDA\x46\x51\x66\x3C\x51\x2B\x82\x09\x09\x09\x20\x34\xD8\x8F\xD8\x88\x65\xAC\x08\x0B\xE2\xF5\x9D\x7A\x76\xE7\x26\xE3\xEB\x17\xC8\xFA\xC4\x04\xD6\xAF\x4F\x14\xE2\xFF\x17\x70\xE5\x67\x72\xE3\x55\xB1\x26\x71\x8D\x58\xAB\x24\x7C\x99\x2F\x51\x21\x12\x1C\x62\xBB\xB7\x5A\x85\xA3\x10\xAA\x55\x76\xB8\x3B\x5B\xBF\xAB\xB9\x38\xE1\xA6\xB2\xC5\xD5\xC5\x96\xEC\x94\x18\x9A\xAA\x77\x52\x55\x92\xC5\xCE\x9C\x64\x52\xD7\xC7\xA3\x09\x0A\xA2\xB7\xBE\x92\x4C\xF9\xDB\x09\x89\xE9\xBB\x8D\x15\x5C\xAC\xA9\x60\x57\x6E\x3A\x25\x5B\x33\x69\x1B\x9A\xE4\xAA\x88\xE8\x14\x5C\x1B\x7B\x66\x82\xF1\xB3\xC2\x58\xFB\xB0\x65\x01\xD2\xF5\x41\xAC\x92\xCE\xD7\xF8\x7A\x9A\xA2\x38\xD8\xDB\x1E\x27\x7B\x4B\x11\xE5\x63\x2A\x8D\xAD\x9D\x92\xA0\x40\x1F\x42\x24\xF5\xCE\x56\xE6\x33\x70\xBE\x9C\x03\x72\xF8\x6C\xCD\x4C\xE1\x64\x4D\x35\x9B\xD2\x53\xE9\xAD\x2D\xE7\xEC\xFE\x7C\x12\xE3\x56\xA2\x6B\x3F\x42\xCF\x99\x52\xEE\xD4\x16\xF3\x71\x69\x0E\xCD\xF7\x46\xE8\x9A\xFA\xCC\x44\x7A\x7D\xFC\xF9\x2F\x50\x68\xC5\xCE\x48\xC9\xEE\xE8\x50\x8D\xD4\xDD\xC7\x94\x5A\x8E\x0E\x12\xA9\x16\x16\xA6\xCE\x2F\xCA\x5A\xCB\xA9\x8A\x3C\x62\xC2\xB5\x04\x06\x06\x90\x9E\x12\xCF\x58\x7B\x15\xD3\x57\xAA\xD0\xB7\x1D\xA4\xAF\x76\x0F\xCF\xBA\x4E\xD2\x7E\xA4\x98\xE1\x96\xC3\xCC\x5C\x3F\xC6\x9A\x98\x08\x06\xCE\x55\x30\x75\xE3\x04\x23\x2D\x07\x38\x5A\xBE\x85\x0B\xB7\x1F\xD2\x3D\xF5\x29\xD7\x46\x67\xB8\x6E\x98\xE5\xC6\xD8\x53\xBA\xC6\xA5\x09\x3D\xE5\x60\x08\xF6\xF3\x42\xEB\xEF\x69\xEA\x01\x95\x44\xAB\xBB\xA3\xB9\xE4\xBA\xA5\xA4\x9B\x92\xED\x22\x60\x67\xF6\x1A\xB2\x52\xE3\x08\x08\xF0\xC3\xD7\xC7\x9B\x07\x4D\x07\xF8\x7E\xE8\x12\xFF\xFA\xA8\x99\xBF\x8C\xB4\xF3\xC3\x50\x1B\x6F\x06\x1B\x64\x47\x74\x32\x7B\xAD\x9A\x8C\x94\x38\xAA\x4B\x72\x4D\xB9\x70\x4B\x5C\x38\xBB\xFF\x43\xDA\xEE\x3E\xA2\x7B\xE2\x39\xD7\x74\xD3\x5C\x17\x11\x37\x44\x44\x97\x88\x50\xB8\x8B\x00\x6F\x89\x52\x3B\x3B\x5B\xE6\xCD\x9D\xC3\xFC\x79\xF3\x89\x88\x8E\xC6\x5D\x2D\xA9\x27\x0D\x98\x20\xF1\x1B\x1B\xA9\x25\x4D\xBA\x39\x2E\x26\x1C\x47\x95\x0B\x15\xDB\x36\xF2\xED\xBD\x06\x53\x06\x7C\xFF\xF8\x12\x3F\xE9\x2F\xF3\xDD\xE3\x16\xFE\xFC\xA0\x99\xE9\xCE\x6A\x8A\x73\x93\x4C\x22\xBE\x7B\x78\x89\xD1\xD6\x43\x9C\xAC\xFC\x90\xCB\x8F\x0C\x26\x01\x46\xE2\x6E\x59\xF9\x7B\x28\xAC\xCC\xCD\x30\x5F\xBC\x90\xC5\x92\x80\x01\xC1\x21\x94\x1D\x3D\xCD\x55\xFD\x13\xB2\xF3\xF3\xB0\xB7\x5A\x28\x2E\x48\xC4\x46\x85\xB1\x29\x25\x96\x84\xD8\x70\x3C\xBD\xBC\x49\x88\x5F\x49\xC3\xC7\xF9\xB2\x0D\x4F\xF0\xA3\xAE\x45\xF2\xA0\x4D\x4A\x72\x84\xD7\xB7\xEB\xA8\x96\x7C\xC8\xDF\xB4\x96\xC6\x83\x1F\xF2\xAA\xBF\x8E\x9F\x86\x9A\x68\x3B\xB9\x87\x8E\xC7\x93\xDC\x14\xC2\xAE\x51\x11\x20\x0D\xD8\x2D\xAB\xEF\x91\xAB\x62\xC3\xD6\x6D\xE4\x95\xEE\x93\xA1\xA1\x8D\x2B\xC3\x93\xF4\x4E\x7F\xCE\xB1\x86\x26\x34\xD2\x0F\x5E\xAE\x36\x12\x48\xF6\xA8\x5D\xE4\x50\x8A\xD1\xA2\x0D\xF2\xC1\xC3\xD3\x1D\xB5\xBB\x9A\x75\x72\x40\x55\x09\x59\x6F\xED\x5E\x06\xCF\xEE\x41\x77\xE9\x00\x53\x97\xAB\xE8\x3E\x59\xC2\x47\xC5\xD9\xEC\x93\x39\xA1\x54\x4E\xD1\x2B\x87\xC4\xFE\xC6\x1A\x6E\x48\x00\xF5\x88\xF5\x3D\xD2\x84\xBD\x46\x88\x80\x5B\xE3\x52\x82\x3E\x21\xEC\x17\xF4\x4E\xBC\xA0\xF3\xD1\x38\x37\x86\x9F\xD0\x78\xA3\x8F\x20\x1F\x3B\xBC\xDC\xAC\x25\xF7\xCD\xE5\x6A\x85\xB9\x85\xB9\xA4\xA5\x92\x15\xC1\x5E\x04\xF8\xFB\xE1\xE7\xE7\x4B\x8C\x34\x5B\x5A\x4A\x02\x97\x8F\x14\x99\xAC\x7F\xD8\xB4\x5F\x82\xA8\x9C\x63\xC5\x49\x6C\x88\xF3\x25\x73\x5D\x38\xBB\xD3\x34\xD4\xD4\x7C\x44\xFF\xCC\x17\xF4\x8D\xCD\x30\x20\xC4\xEF\xD1\x2F\x50\x5C\x13\xD2\xAB\x82\x6B\x8F\x85\x7C\x64\x92\xEB\xC3\x13\x74\xE9\xA7\x38\xDE\xD8\x44\x4C\xA4\x3F\x2B\x23\x03\x65\xEF\x3B\x31\x57\x7A\xC3\x43\x9C\xC8\x88\x72\x66\xDD\x4A\x1F\x34\x1A\x0D\xE1\xA1\x5A\xA2\xA2\x23\xE8\xAD\xDB\xCD\x84\xEC\x8C\x43\xDB\x92\xC9\x8C\x51\x11\x13\xA2\x22\x37\x29\x82\x03\xDB\x37\xB1\x76\xD5\x32\x7C\xBC\x1C\x68\xE9\xBD\xCB\x9D\xA9\x67\xF4\x8D\x4E\x30\x30\x3E\x25\x30\x5E\x27\x51\x74\xEB\xA7\xE9\xD6\x4D\xBD\xC3\x88\x40\xCA\x70\x43\xC4\xF4\xE8\x66\xE8\x1E\x1A\xA3\xBE\xF3\x2A\xB6\x36\x36\xCC\x9F\x3F\x9F\x25\x72\x10\x69\x97\x2A\x29\x8C\x73\x61\x5D\x94\x06\xFF\x40\x09\xAE\xD0\x60\xAE\x89\xED\xCD\x07\x0A\xD8\x93\xEC\x4E\x72\x98\x03\x07\x4B\xB6\xD0\x79\xBC\x8C\x43\xC5\x9B\xA9\x2C\xCA\x21\x5C\xE3\x4A\x55\xED\x69\x06\xA7\x9E\x70\x6B\x74\x94\xFE\xF1\x71\x6E\x4F\x4C\x32\x30\x31\x21\x25\x18\x9B\xA4\xDF\x30\xC9\x2D\x59\xF5\x2D\xFD\x18\xBD\x3A\x83\x09\xDD\x23\x06\xAE\xF4\xF5\x50\x98\x9F\xC6\xBC\xF9\x0B\x24\x8E\xAD\xB1\x94\x71\xCD\x41\x69\xC3\x72\x3F\x07\x8A\x13\x9C\x24\xB2\xDD\x88\x91\x06\xAD\x96\x71\x2D\x31\xC2\x9D\x8D\x51\x6E\xA4\xC4\x86\xF0\x3F\x72\x1C\x3F\xBD\x79\x46\xC8\x33\x88\x8F\x0A\x62\x43\x6A\xBC\xF4\x55\x1D\x57\x06\x6F\x9B\x56\x3E\x28\xE4\x83\x13\x53\xDC\x11\x28\xFA\x0D\x63\xA6\x87\xFD\x63\xE3\x52\xA3\x31\xFA\xE4\x73\x8F\x4E\xC7\x9D\x27\xB3\x14\x16\x17\x11\xA5\xF5\xC0\x5A\x1C\x30\x92\xAB\x64\x9A\xF1\xF6\x72\xC7\x41\xA2\x3B\x39\xD4\x8E\xDD\x89\x4E\x6C\x49\xF4\x65\xDB\x7A\x3F\x36\xAE\x52\xE3\x25\x56\x27\xAD\x0E\x61\x50\x42\xA8\xF9\xA3\x6C\x3E\xCE\x8F\x91\xF1\xCD\x85\x42\x99\x94\x6F\x1B\xC6\x19\x10\xDC\x11\xDB\x8D\xC4\x46\x0C\x1A\x4B\xD0\xAB\x17\x4B\x84\xBC\x5F\xFE\x78\x7B\x4C\xEA\x22\xD7\x5B\xF2\xAC\xF3\xDE\x7D\xD6\x27\x84\xCB\x19\x21\x93\x6D\xC8\x52\x71\x40\x46\x30\x39\x25\xA3\x65\xC5\x39\x9B\x8C\x53\x8E\x15\x61\xFE\xF6\xE4\xC7\xDB\x53\xBA\xD6\x91\xC2\x78\x27\x56\x87\x39\x91\x19\xAD\x22\x2F\xC1\x5D\xCA\xE1\x46\x4D\xA6\x27\xA7\xB2\x7D\x28\x5C\xA3\xA6\x43\xDC\xBC\x3F\x3D\x6B\xE2\x30\x12\x9B\x04\x88\xFB\x8A\x5B\xBA\x51\x13\xA1\xF1\xDA\xA7\x37\x98\x70\x57\x6A\x75\xAE\xF3\x1A\xF5\x87\xF3\xB9\x71\xBE\x84\x8C\xB5\xA1\xEC\x92\x1F\x1C\xC1\x32\xA4\xAA\x5C\x5D\x68\x39\xB6\xD3\x94\x90\x16\x32\x21\x69\x65\x3E\xD8\xB8\x42\xC9\xE6\x68\x41\x8C\x08\x88\x71\x23\x5D\xC6\xBC\x8D\xD1\x2E\x64\xC4\xA8\xC9\x58\xA5\x22\x25\xDC\x8A\x8A\xF2\x6D\x3C\x78\xF2\x8C\x41\x71\xF8\x9E\x34\xE1\x3D\x11\x70\xCF\xE8\x80\x49\x80\xA0\x77\x44\x4F\xF7\xA3\x21\x13\x1E\xCE\x3C\x23\x31\x6D\x3D\x21\x1A\x67\x7E\x3F\xD4\xC8\x27\x1D\x7B\xF9\xA8\x28\x81\xEA\xBD\x99\x64\x6E\x58\xC7\xF6\xBC\x14\xEE\x34\x54\x92\x91\x1C\xC3\x32\xAD\x0C\xAA\x72\x8E\x38\x39\xD9\x11\xE0\x65\x23\x3B\x40\xC9\xEA\xE5\x2A\x52\x57\xC9\x84\x1C\xA6\x26\x72\x55\x24\xDE\x32\x3F\x44\xAF\x5C\xC6\xDD\x89\x71\xEE\x8E\x8B\x80\xC9\x09\xEE\xFF\x0C\x45\xEF\x90\x8E\x9E\xA1\x11\x7A\x1E\x0F\x73\x6B\x58\xCF\xE3\xA7\x2F\x28\xDB\x5F\xC9\x6F\x7F\xA3\xE0\x57\xFF\xA2\x20\x54\x44\x4C\x0D\x9E\x64\xAA\xEF\x88\x4C\x41\x09\x34\x1F\x2B\xA0\xA1\xA6\x98\x6B\xB5\x15\x34\x1C\xDC\x4E\x9E\xA4\x5E\xE7\x89\x5D\x5C\x3B\xBE\x93\x9D\x12\xC1\x39\xA9\x31\xD2\x0B\xAE\xB8\xB9\x2A\x71\x71\xB0\x60\xCD\xDA\x38\xB6\x6E\x2F\x22\x6D\x63\x1A\xBA\x99\x59\xEE\x8B\xDB\x9F\x8C\x1A\x78\x20\x65\x7F\x20\x6E\x28\xFA\x47\x46\x18\x94\x87\x03\xD2\x78\x57\xFA\xFB\x38\x5C\x73\x02\x2B\xEB\xF9\xCC\xF9\x40\xC1\xE2\x85\xFF\x6C\x12\xB1\x54\x56\x76\xFB\xF2\x21\xFE\x38\x73\x95\xD9\x4F\x6A\x99\xB9\x7F\x96\xC7\x37\x6B\xB8\x79\xA9\x8A\x0B\x27\x4A\x68\xA9\x2E\xE6\x85\x9C\x88\xDF\xCB\xE1\xD4\x7B\x76\x2F\x99\x1B\x57\x13\xB8\xD4\x19\x5F\x0F\x67\x34\xCB\x34\x5C\xEF\xEF\xE5\xC1\xA8\x8E\x47\x63\xA3\x3C\x32\xC8\x22\xE5\xFA\x78\x5C\xEE\xC7\xF4\x28\x06\x86\x86\x68\x6C\x6D\x23\x76\x75\x82\xCC\xF8\x6A\x94\x8B\x2D\xF0\x97\x5F\x2E\x36\xE6\xBF\x63\xDE\xBC\x7F\x14\x11\xFF\x24\xBF\x13\xFE\x41\x0E\xAB\xDF\xD2\x78\x6A\x37\x7F\xF9\xC6\xC0\x7F\x7C\x37\xCB\x8F\x7F\x30\xF0\xA7\xDF\x3F\xE0\xED\xF3\x41\xA6\xEE\x5F\x60\xEC\x76\x03\x93\x3D\x75\x34\x4B\x2A\xE6\x65\xA7\x70\xAB\xFD\x38\x47\xF7\xA4\x73\xE2\xCC\x29\x0C\x4F\x9F\x88\x80\x11\x13\x1E\x1A\x74\x26\x72\x93\x80\x71\x3D\xFF\x07\xBC\x56\x64\x24\x3B\xC4\x6A\x82\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82";

        auto custom_ponce_icon = load_custom_icon(ponce_icon, sizeof(ponce_icon), "png");
        update_action_icon(ctx->action, custom_ponce_icon);

        return AST_ENABLE_ALWAYS;
    }
};
static ah_ponce_banner_t ah_ponce_banner;

//We need to define this struct before the action handler because we are using it inside the handler
action_desc_t action_IDA_ponce_banner = ACTION_DESC_LITERAL(
    "Ponce:banner",
    "Ponce Plugin", //The action text.
    &ah_ponce_banner, //The action handler.
    NULL, //Optional: the action shortcut
    "Use settings below while debugging", //Optional: the action tooltip (available in menus/toolbar)
    12); //Optional: the action icon (shows when in menus/toolbars)


#if IDA_SDK_VERSION >= 740
/*	Adapted code from https://forum.hex-rays.com/viewtopic.php?f=8&t=4161&p=19146#p19146
    Need to test and complete with IDA 7.4 and IDA 7.5 whenver they are released*/
struct ah_ponce_symbolize_reg_UI_t : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx)
    {
        auto reg_name = ctx->regname;
        uint64 reg_value;
        get_reg_val(reg_name, &reg_value);
        register_info_t rinfo;
        char value_str[16] = { 0 };
        char* type = "integer";
        if (get_dbg_reg_info(reg_name, &rinfo)) {
            if (rinfo.dtype == dt_byte)
                qsnprintf(value_str, sizeof(value_str), "0x%02x", reg_value);
            else if (rinfo.dtype == dt_byte)
                qsnprintf(value_str, sizeof(value_str), "0x%02x", reg_value);
            else if (rinfo.dtype == dt_byte)
                qsnprintf(value_str, sizeof(value_str), "0x%02x", reg_value);
            else if (rinfo.dtype == dt_byte)
                qsnprintf(value_str, sizeof(value_str), "0x%02x", reg_value);
            else
                type = "float";
        }
        msg("> Register %s (of type %s): %s\n", reg_name, type, value_str);

        symbolize_register(reg_name, ctx);

        return 0;
    }

    virtual action_state_t idaapi update(action_update_ctx_t* ctx)
    {
        if (ctx->widget_type == BWN_CPUREGS) {
            return AST_ENABLE_FOR_WIDGET;
        }
        else {
            return AST_DISABLE_FOR_WIDGET;
        }
    }
};
static ah_ponce_symbolize_reg_UI_t ah_ponce_symbolize_reg_UI;

//We need to define this struct before the action handler because we are using it inside the handler
action_desc_t action_IDA_ponce_symbolize_reg = ACTION_DESC_LITERAL(
    "registers_context_menu:dump_reg_name",
    "Dump register name from registers window", //The action text.
    &ah_ponce_symbolize_reg_UI, //The action handler.
    NULL, //Optional: the action shortcut
    "Right click and symbolize register", //Optional: the action tooltip (available in menus/toolbar)
    65); //Optional: the action icon (shows when in menus/toolbars)
#endif


/*This list defined all the actions for the plugin*/
struct action action_list[] =
{
    { &action_IDA_ponce_banner, { BWN_DISASM, __END__ }, true, true, "" },

    { &action_IDA_enable_disable_tracing, { BWN_DISASM, __END__ }, true, true, "" },

    { &action_IDA_taint_register, { BWN_DISASM, BWN_CPUREGS, __END__ }, true, false, "Taint/"},
    { &action_IDA_taint_memory, { BWN_DISASM, BWN_DUMP, __END__ }, true, false, "Taint/" },

    { &action_IDA_symbolize_register, { BWN_DISASM, BWN_CPUREGS, __END__ }, false, true, "Symbolic/"},
    { &action_IDA_symbolize_memory, { BWN_DISASM, BWN_DUMP, __END__ }, false, true, "Symbolic/" },

    { &action_IDA_negate, { BWN_DISASM, __END__ }, false, true, "SMT/" },
    { &action_IDA_negateInjectRestore, { BWN_DISASM, __END__ }, true, true, "SMT/" },

    { &action_IDA_createSnapshot, { BWN_DISASM, __END__ }, true, true, "Snapshot/"},
    { &action_IDA_restoreSnapshot, { BWN_DISASM, __END__ }, true, true, "Snapshot/" },
    { &action_IDA_deleteSnapshot, { BWN_DISASM, __END__ }, true, true, "Snapshot/" },
    { &action_IDA_execute_native, { BWN_DISASM, __END__ }, true, true, "" },

#if IDA_SDK_VERSION >= 740
    { &action_IDA_ponce_symbolize_reg, { BWN_CPUREGS, __END__ }, true, true, "" },
#endif

    { NULL, __END__, __END__ }
};
