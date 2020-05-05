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
#include "callbacks.hpp"
#include "actions.hpp"
#include "globals.hpp"
#include "trigger.hpp"
#include "context.hpp"
#include "utils.hpp"
#include "formConfiguration.hpp"
#include "triton_logic.hpp"
#include "actions.hpp"

//Triton
#include <triton/api.hpp>

/*This function is called once in the IDA plugin init event to set the static configuration for triton. Architecture and memory/registry callbacks.*/
//void triton_init()
//{	
//	//We need to set the architecture for Triton
//	api.setArchitecture(TRITON_ARCH);
//	// Memory access callback
//	api.addCallback(needConcreteMemoryValue);
//	// Register access callback
//	api.addCallback(needConcreteRegisterValue);
//	// This optimization is veeery good for the size of the formulas
//	api.enableSymbolicOptimization(triton::engines::symbolic::ALIGNED_MEMORY, true);
//	// We only are symbolic or taint executing an instruction if it is tainted, so it is a bit faster and we save a lot of memory
//	api.enableSymbolicOptimization(triton::engines::symbolic::ONLY_ON_SYMBOLIZED, true);
//	api.enableSymbolicOptimization(triton::engines::symbolic::ONLY_ON_TAINTED, true);
//}

// "C:\Program Files\IDA 7.1\ida64.exe" -A "C:\Users\default.DESKTOP-Q4FDM2G\Documents\code\Ponce\build_x64\Debug\crackme_xor.exe"

//--------------------------------------------------------------------------

bool idaapi run(size_t)
{
    /*We shouldn't prompt for it if the user has a saved configuration*/
    if (!load_options(&cmdOptions)) {
        prompt_conf_window();
    }

    if (!hooked) {
        //Registering action for the Ponce config
        register_action(action_IDA_show_config);
        attach_action_to_menu("Edit/Ponce/", action_IDA_show_config.name, SETMENU_APP);
        //Registering action for the Ponce taint window
        register_action(action_IDA_show_taintWindow);
        attach_action_to_menu("Edit/Ponce/", action_IDA_show_taintWindow.name, SETMENU_APP);
        //Registering action for the unload action
        register_action(action_IDA_unload);
        attach_action_to_menu("Edit/Ponce/", action_IDA_unload.name, SETMENU_APP);
        //Some actions needs to use the api and the api need to have the architecture set
        if (!ponce_set_triton_architecture()) {
            return false;
        }

        // Set the name for the action depending if using tainting or symbolic engine
        if (cmdOptions.use_tainting_engine) {
            action_list[2].menu_path = TAINT;
            action_IDA_taint_symbolize_register.label = TAINT_REG;
            action_IDA_taint_symbolize_register.tooltip = COMMENT_TAINT_REG;

            action_list[3].menu_path = TAINT;
            action_IDA_taint_symbolize_memory.label = TAINT_MEM;
            action_IDA_taint_symbolize_memory.tooltip = COMMENT_TAINT_MEM;
#if IDA_SDK_VERSION >= 740
            action_list[10].menu_path = TAINT;
            action_IDA_ponce_symbolize_reg.label = TAINT_REG;
            action_IDA_ponce_symbolize_reg.tooltip = COMMENT_TAINT_REG;
#endif
        }
        else {
            action_list[2].menu_path = SYMBOLIC;
            action_IDA_taint_symbolize_register.label = SYMBOLICE_REG;
            action_IDA_taint_symbolize_register.tooltip = COMMENT_SYMB_REG;

            action_list[3].menu_path = SYMBOLIC;
            action_IDA_taint_symbolize_memory.tooltip = COMMENT_SYMB_MEM;
            action_IDA_taint_symbolize_memory.label = SYMBOLICE_MEM;
#if IDA_SDK_VERSION >= 740
            action_list[10].menu_path = SYMBOLIC;
            action_IDA_ponce_symbolize_reg.label = SYMBOLICE_REG;
            action_IDA_ponce_symbolize_reg.tooltip = COMMENT_SYMB_REG;
#endif          
        }

        //Loop to register all the actions used in the menus
        for (int i = 0;; i++) {
            if (action_list[i].action_decs == NULL) {
                break;
            }
            //Here we register all the actions
            if (!register_action(*action_list[i].action_decs)) {
                warning("[!] Failed to register %s actions. Exiting Ponce plugin\n", action_list[i].action_decs->name);
                return false;
            }
        }

        if (!hook_to_notification_point(HT_UI, ui_callback, NULL)) {
            warning("[!] Could not hook ui callback");
            return false;
        }
        if (!hook_to_notification_point(HT_DBG, tracer_callback, NULL)) {
            warning("[!] Could not hook tracer callback");
            return false;
        }

        msg("[+] Ponce plugin version: %s running!\n", VERSION);
        hooked = true;
    }
    return true;
}

//--------------------------------------------------------------------------
#if IDA_SDK_VERSION >= 750
size_t idaapi init(void)
#else
int idaapi init(void)
#endif
{
    char version[8];
    //We do some checks with the versions...
    if (get_kernel_version(version, sizeof(version))) {
#if IDA_SDK_VERSION > 750
        warning("[!] This Ponce plugin was built for IDA %d, you are using: %s\n", IDA_SDK_VERSION, version);
#elif IDA_SDK_VERSION == 750
        if (strcmp(version, "7.5") != 0)
        warning("[!] This Ponce plugin was built for IDA %d, you are using: %s\n", IDA_SDK_VERSION, version);       
#elif IDA_SDK_VERSION == 740
        if (strcmp(version, "7.4") != 0)
            warning("[!] This Ponce plugin was built for IDA %d, you are using: %s\n", IDA_SDK_VERSION, version);
#elif IDA_SDK_VERSION == 730
        if (strcmp(version, "7.3") != 0)
            warning("[!] This Ponce plugin was built for IDA %d, you are using: %s\n", IDA_SDK_VERSION, version);
#elif IDA_SDK_VERSION == 720
        if (strcmp(version, "7.2") != 0)
            warning("[!] This Ponce plugin was built for IDA %d, you are using: %s\n", IDA_SDK_VERSION, version);
#elif IDA_SDK_VERSION == 710
        if (strcmp(version, "7.1") != 0)
            warning("[!] This Ponce plugin was built for IDA %d, you are using: %s\n", IDA_SDK_VERSION, version);
#elif IDA_SDK_VERSION == 700
        if (strcmp(version, "7.00") != 0)
            warning("[!] This Ponce plugin was built for IDA %d, you are using: %s\n", IDA_SDK_VERSION, version);
#elif IDA_SDK_VERSION < 700
#error // not supported
#endif
    }

    if (int(version[0]) < 7) {
        warning("[!] Ponce plugin can't run with IDA version < 7. Please use a newer IDA version");
        return PLUGIN_SKIP;
    }
    //Error loading config?
    if (!load_options(&cmdOptions))
        return PLUGIN_KEEP;
    //We want to autorun the plugin when IDA starts?
    if (cmdOptions.auto_init)
        run(0);
    return PLUGIN_KEEP;
}

//--------------------------------------------------------------------------
void idaapi term(void)
{
    // remove snapshot if exists
    snapshot.resetEngine();
    // We want to delete Ponce comments and colours before terminating
    delete_ponce_comments();
    // Unhook notifications
    unhook_from_notification_point(HT_UI, ui_callback, NULL);
    unhook_from_notification_point(HT_DBG, tracer_callback, NULL);
    // Unregister and detach menus
    unregister_action(action_IDA_show_config.name);
    detach_action_from_menu("Edit/Ponce/", action_IDA_show_config.name);
    unregister_action(action_IDA_show_taintWindow.name);
    detach_action_from_menu("Edit/Ponce/", action_IDA_show_taintWindow.name);
    unregister_action(action_IDA_unload.name);
    detach_action_from_menu("Edit/Ponce/", action_IDA_unload.name);
    detach_action_from_menu("Edit/Ponce/", "");
    hooked = false;
}

//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN =
{
    IDP_INTERFACE_VERSION,
    0,                    // plugin flags
    init,                 // initialize
    term,                 // terminate. this pointer may be NULL.
    run,                  // invoke plugin
    "Ponce, a concolic execution plugin for IDA", // long comment about the plugin
    "", // multiline help about the plugin
    "Ponce", // the preferred short name of the plugin
    "Ctrl+Shift+Z" // the preferred hotkey to run the plugin
};

