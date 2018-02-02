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

//Triton
#include <api.hpp>

/*This function is called once in the IDA plugin init event to set the static configuration for triton. Architecture and memory/registry callbacks.*/
/*void triton_init()
{
	//We need to set the architecture for Triton
	triton::api.setArchitecture(TRITON_ARCH);
	// Memory access callback
	triton::api.addCallback(needConcreteMemoryValue);
	// Register access callback
	triton::api.addCallback(needConcreteRegisterValue);
	// This optimization is veeery good for the size of the formulas
	triton::api.enableSymbolicOptimization(triton::engines::symbolic::ALIGNED_MEMORY, true);
	// We only are symbolic or taint executing an instruction if it is tainted, so it is a bit faster and we save a lot of memory
	triton::api.enableSymbolicOptimization(triton::engines::symbolic::ONLY_ON_SYMBOLIZED, true);
	triton::api.enableSymbolicOptimization(triton::engines::symbolic::ONLY_ON_TAINTED, true);
}*/

//--------------------------------------------------------------------------

#ifdef __IDA70__
bool idaapi run(size_t)
#else
void idaapi run(int)
#endif
{
	/*We shouldn't prompt for it if the user has a saved configuration*/
	if (!load_options(&cmdOptions))
	{
		prompt_conf_window();
	}

	if (!hooked)
	{
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
		triton::api.setArchitecture(TRITON_ARCH);
		
		//Loop to register all the actions used in the menus
		for (int i = 0;; i++)
		{
			if (action_list[i].action_decs == NULL){
				break;
			}
			//Here we register all the actions
			if (!register_action(*action_list[i].action_decs))
			{
				warning("[!] Failed to register %s actions. Exiting Ponce plugin\n", action_list[i].action_decs->name);
#ifdef __IDA70__
				return false;
#else
				return;
#endif
			}
		}


		//First we ask the user to take a snapshot, -1 is to cancel so we don't run the plugin
		if (ask_for_a_snapshot() != -1)
		{
			if (!hook_to_notification_point(HT_UI, ui_callback, NULL))
			{
				warning("[!] Could not hook ui callback");
#ifdef __IDA70__
				return false;
#else
				return;
#endif
			}
			if (!hook_to_notification_point(HT_DBG, tracer_callback, NULL))
			{
				warning("[!] Could not hook tracer callback");
#ifdef __IDA70__
				return false;
#else
				return;
#endif
			}
		
			msg("[+] Ponce plugin version: %s running!\n", VERSION);
			hooked = true;
		}
	}
#ifdef __IDA70__
	return true;
#else
	return;
#endif
}

//--------------------------------------------------------------------------
int idaapi init(void)
{
	char version[8];
	//We do some checks with the versions...
	if (get_kernel_version(version, sizeof(version)))
	{
#ifdef __IDA70__
		//The IDA 7.0 plugin running in old IDA
		if (strcmp(version, "7.00") != 0)
			warning("[!] This Ponce plugin was built for IDA 7.0, you are using: %s\n", version);
#elif __IDA68__
		//The IDA 6.8 plugin running in IDA 6.9x
		if (strcmp(version, "6.8") != 0)
			warning("[!] This plugin was built for IDA 6.8, you are using: %s\n", version);
#else
		//The IDA 6.9x plugin running in IDA 6.8
		if (strcmp(version, "6.8") == 0)
			warning("[!] This plugin was built for IDA 6.9x, you are using: %s\n", version);
#endif
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
