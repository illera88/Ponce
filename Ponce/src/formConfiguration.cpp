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
#include <pro.h>
#include <idp.hpp>
#include <loader.hpp>
#include <kernwin.hpp>

//Ponce
#include "formConfiguration.hpp"
#include "globals.hpp"
#include "utils.hpp"

//--------------------------------------------------------------------------
//This function is used to activate or deactivate other items in the form while using it
int idaapi modcb(int fid, form_actions_t &fa)
{
	ushort isActivated=0;
	//Symbolic indexing still not implemented
	fa.enable_field(15, 0);

	//The Taint recv and Taint fread are disabled until we implemented them
	fa.enable_field(12, 0);
	fa.enable_field(13, 0);
	switch (fid)
	{
	case -1:
		fa.get_checkbox_value(8, &isActivated);
		fa.enable_field(9, isActivated ? 1 : 0);
		fa.enable_field(10, isActivated ? 1 : 0);
		fa.enable_field(11, isActivated ? 1 : 0);
		break;
	case -2:
		break;
	case 4:
		fa.get_checkbox_value(4, &isActivated);
		fa.enable_field(22, isActivated ? 1 : 0);
		break;
	case 8: // Depending if argv(8) is activated we should activate 9, 10 and 11
		fa.get_checkbox_value(8, &isActivated);
		fa.enable_field(9, isActivated ? 1 : 0);
		fa.enable_field(10, isActivated ? 1 : 0);
		fa.enable_field(11, isActivated ? 1 : 0);
		break;
	default:
		break;
	}
	return 1;
}

/*IDA windows style is a FUMADA!! Will move soon to Qt :)
I you want to create a new checkbox you should:
- add it to the form variable that its in formChoser.hpp
- Create the global variable in cmdOptions
- if you want to inizialize it to something do it in the first if of the function (if (!cmdOptions.already_configured))
- in the else you should add the correspondant item in the chkgroup where you are adding the item
- if AskUsingForm_c success (user clicks accept) set the value of your item from his correspondent chkgroupX
- add it to the msg at the end of the function for debug purposes*/
void prompt_conf_window(void){
	/*We should create as many ushort variables as groups of checkboxes we have in the form window*/
	ushort chkgroup1, chkgroup2, chkgroup3;
	ushort symbolic_or_taint_engine = 0;

	if (!cmdOptions.already_configured){
		//Here we can initialize the checkboxes by group
		chkgroup1 = 2;
		chkgroup2 = 1 | 2 | 8 | 64;
		chkgroup3 = 1 | 2 | 8;

		cmdOptions.blacklist_path[0] = '\0'; // Will use this to check if the user set some path for the blacklist
	}
	else{
		/*By default all the variables are set to false. If the user wants to change the configuration
		after using the plugin we should set the variables to the previously set configuration. If we
		don't do this the variables will be always initialized to  the previous lines
		NOTE: Parenthesis are mandatory or it won't work!*/
		chkgroup1 = (cmdOptions.auto_init ? 1 : 0) | (cmdOptions.showDebugInfo ? 2 : 0) | (cmdOptions.showExtraDebugInfo ? 4 : 0);
		chkgroup2 = (cmdOptions.taintArgv ? 1 : 0) | (cmdOptions.taintEndOfString ? 2 : 0) | (cmdOptions.taintArgv0 ? 4 : 0) | (cmdOptions.taintArgc ? 8 : 0) | (cmdOptions.taintRecv ? 16 : 0) | (cmdOptions.taintFread ? 32 : 0) | (cmdOptions.only_on_optimization ? 64: 0) | (cmdOptions.manageSymbolicIndexing ? 128 : 0);
		chkgroup3 = (cmdOptions.addCommentsControlledOperands ? 1 : 0) | (cmdOptions.RenameTaintedFunctionNames ? 2 : 0) | (cmdOptions.addCommentsSymbolicExpresions ? 4 : 0) | (cmdOptions.paintExecutedInstructions ? 8 : 0);

		symbolic_or_taint_engine = cmdOptions.use_symbolic_engine ? 0 : 1;	
	}
#ifdef __IDA70__
	if (ask_form(form,
		modcb, // the call to this function can be omitted. It's only usefull if a checkbox activate or dissable other elements of the form
		&cmdOptions.limitTime,
		&cmdOptions.limitInstructionsTracingMode,
		&symbolic_or_taint_engine,
		&chkgroup1,
		&chkgroup2,
		&chkgroup3,
		&cmdOptions.color_tainted,
		&cmdOptions.color_executed_instruction,
		&cmdOptions.color_tainted_condition,
		&cmdOptions.blacklist_path
		) > 0)
#else
	if (AskUsingForm_c(form,
		modcb, // the call to this function can be omitted. It's only usefull if a checkbox activate or dissable other elements of the form
		&cmdOptions.limitTime,
		&cmdOptions.limitInstructionsTracingMode,
		&symbolic_or_taint_engine,
		&chkgroup1,
		&chkgroup2,
		&chkgroup3,
		&cmdOptions.color_tainted,
		&cmdOptions.color_executed_instruction,
		&cmdOptions.color_tainted_condition,
		&cmdOptions.blacklist_path
		) > 0)
#endif
	{
		/*First we set the flag that indicates that the user already provided a configuration
		so we should't use the default values anymore but the ones that the user provided previously*/
		cmdOptions.already_configured = true;

		// Get the user selection for the exclusive checkboxes
		switch (symbolic_or_taint_engine) {
			case 0: // Symbolic engine
				cmdOptions.use_symbolic_engine = true;
				cmdOptions.use_tainting_engine = false;
				break;
			case 1: // Tainting engine
				cmdOptions.use_tainting_engine = true;
				cmdOptions.use_symbolic_engine = false;
				break;
		}

		/*Now that the user pressed accept we need to transform the chkgroups to actual booleans for cmdOptions*/
		cmdOptions.auto_init = chkgroup1 & 1 ? 1 : 0;
		cmdOptions.showDebugInfo = chkgroup1 & 2 ? 1 : 0;
		cmdOptions.showExtraDebugInfo = chkgroup1 & 4 ? 1 : 0;
		//
		cmdOptions.taintArgv = chkgroup2 & 1 ? 1 : 0;
		cmdOptions.taintEndOfString = chkgroup2 & 2 ? 1 : 0;
		cmdOptions.taintArgv0 = chkgroup2 & 4 ? 1 : 0;
		cmdOptions.taintArgc = chkgroup2 & 8 ? 1 : 0;
		cmdOptions.taintRecv = chkgroup2 & 16 ? 1 : 0;
		cmdOptions.taintFread = chkgroup2 & 32 ? 1 : 0;
		cmdOptions.only_on_optimization = chkgroup2 & 64 ? 1 : 0;
		cmdOptions.manageSymbolicIndexing = chkgroup2 & 128 ? 1 : 0;
		//
		cmdOptions.addCommentsControlledOperands = chkgroup3 & 1 ? 1 : 0;
		cmdOptions.RenameTaintedFunctionNames = chkgroup3 & 2 ? 1 : 0;
		cmdOptions.addCommentsSymbolicExpresions = chkgroup3 & 4 ? 1 : 0;
		cmdOptions.paintExecutedInstructions = chkgroup3 & 8 ? 1 : 0;

		if (cmdOptions.blacklist_path[0] != '\0'){
			//Means that the user set a path for custom blacklisted functions
			if (blacklkistedUserFunctions != NULL){
			//Check if we had a previous custom blacklist and if so we delete it
				blacklkistedUserFunctions->clear();
				delete blacklkistedUserFunctions;
				blacklkistedUserFunctions = NULL;
			}
			readBlacklistfile(cmdOptions.blacklist_path);
		}


		save_options(&cmdOptions);
		if (cmdOptions.showDebugInfo){
			msg("\n"
				"limitTime: %lld\n"
				"limitInstructionsTracingMode: %lld\n"
				"use_symbolic_engine: %s\n"
				"auto_init: %s\n"
				"showDebugInfo: %s\n"
				"showExtraDebugInfo: %s\n"
				"taintArgv: %s\n"
				"taintEndOfString: %s\n"
				"taintArgv0: %s\n"
				"taintArgc: %s\n"
				"taintRecv: %s\n"
				"taintFread: %s\n"
				"only_on_optimization: %s\n"
				"manageSymbolicIndexing: %s\n"
				"addCommentsControlledOperands: %s\n"
				"RenameTaintedFunctionNames: %s\n"
				"addCommentsSymbolicExpresions: %s\n"
				"paintExecutedInstructions: %s\n"
				"color_tainted: %x\n"
				"color_tainted_execution: %x\n"
				"color_tainted_condition: %x\n",
				cmdOptions.limitTime,
				cmdOptions.limitInstructionsTracingMode,
				cmdOptions.use_symbolic_engine ? "symbolic engine enabled" : "tainting engine enabled",
				cmdOptions.auto_init ? "true" : "false",
				cmdOptions.showDebugInfo ? "true" : "false",
				cmdOptions.showExtraDebugInfo ? "true" : "false",
				cmdOptions.taintArgv ? "true" : "false",
				cmdOptions.taintEndOfString ? "true" : "false",
				cmdOptions.taintArgv0 ? "true" : "false",
				cmdOptions.taintArgc ? "true" : "false",
				cmdOptions.taintRecv ? "true" : "false",
				cmdOptions.taintFread ? "true" : "false",
				cmdOptions.only_on_optimization ? "true" : "false",
				cmdOptions.manageSymbolicIndexing ? "true" : "false",
				cmdOptions.addCommentsControlledOperands ? "true" : "false",
				cmdOptions.RenameTaintedFunctionNames ? "true" : "false",
				cmdOptions.addCommentsSymbolicExpresions ? "true" : "false",
				cmdOptions.paintExecutedInstructions ? "true" : "false",
				cmdOptions.color_tainted,
				cmdOptions.color_executed_instruction,
				cmdOptions.color_tainted_condition
				);
		}
	}
}