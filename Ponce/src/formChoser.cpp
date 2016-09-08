/*
*  This plugin demonstrates how to use choosers inside forms.
*
*/

#include <pro.h>
#include <idp.hpp>
#include <loader.hpp>
#include <kernwin.hpp>

//Ponce
#include "formChoser.hpp"
#include "globals.hpp"


//--------------------------------------------------------------------------
//This function is not used since it only works when  we need to activate or deactivate other items in the form while using it
int idaapi modcb(int fid, form_actions_t &fa)
{
	ushort isActivated=0;
	
	switch (fid)
	{
	case -1:
		break;
	case -2:
		break;
	case 29: // Depending if argv(29) is activated we should activate 19 and 24
		fa.get_checkbox_value(fid, &isActivated);
		fa.enable_field(19, isActivated ? 1 : 0);
		fa.enable_field(24, isActivated ? 1 : 0);
		//if (!isActivated){
		//	fa._set_field_value(19,0);
		//	fa._set_field_value(24, 0);
		//}
		break;
	//case 10:     // show debug info
	//	fa.get_checkbox_value(fid, &isActivated);
	//	cmdOptions.showDebugInfo = isActivated ? 1 : 0;
	//	break;
	//case 11:     // manage symbolic indexing
	//	fa.get_checkbox_value(fid, &isActivated);
	//	cmdOptions.manageSymbolicIndexing = isActivated ? 1 : 0;
	//	break;
	//case 12:     // enable tracing first time something is tainted
	//	fa.get_checkbox_value(fid, &isActivated);
	//	cmdOptions.enableTracingAsTainted = isActivated ? 1 : 0;
	//	break;
	//case 17:     // Automatically rename the functions with tainted instructions/conditions
	//	fa.get_checkbox_value(fid, &isActivated);
	//	cmdOptions.RenameFunctionNames = isActivated ? 1 : 0;
	//	break;
	//case 18:     // Configure automated tainting
	//	fa.get_checkbox_value(fid, &isActivated);
	//	cmdOptions.automatedTainting = isActivated ? 1 : 0;
	//	break;
	//case 19:     // taint argv[0]
	//	fa.get_checkbox_value(fid, &isActivated);
	//	cmdOptions.taintArgv0 = isActivated ? 1 : 0;
	//	break;
	//case 24:     // taint argc
	//	fa.get_checkbox_value(fid, &isActivated);
	//	cmdOptions.taintArgc = isActivated ? 1 : 0;
	//	break;
	//case 25:     // taint recv
	//	fa.get_checkbox_value(fid, &isActivated);
	//	cmdOptions.taintRecv = isActivated ? 1 : 0;
	//	break;
	//case 26:     // Taint fread
	//	fa.get_checkbox_value(fid, &isActivated);
	//	cmdOptions.taintFread = isActivated ? 1 : 0;
	//	break;
	default:
		//msg("unknown id %d\n", fid);
		break;
	}
	return 1;
}

/*IDA windows style is a fucking FUMADA!! 
I you want to create a new checkbox you should:
- add it to the form variable that its in formChoser.hpp
- Create the global variable in cmdOptions
- if you want to inizialize it to something do it in the first if of the function (if (!cmdOptions.already_configured))
- in the else you should add the correspondant item in the chkgroup where you are adding the item
- if AskUsingForm_c success (user clicks accept) set the value of your item from his correspondent chkgroupX
- add it to the msg at the end of the function for debug purposes*/
void prompt_conf_window(void){
	/*We should create as many ushort variables as groups of checkboxes we have in the form window*/
	ushort chkgroup1, chkgroup2, chkgroup3, chkgroup4;
	ushort symbolic_or_taint_engine = 0;

	if (!cmdOptions.already_configured){
		//Here we can initialize the checkboxes by group
		chkgroup1 = 1;
		chkgroup2 = 1 | 2;
		chkgroup3 = 1 | 8 | 16;
		chkgroup4 = 1 | 8;
	}
	else{
		/*By default all the variables are set to false. If the user wants to change the configuration
		after using the plugin we should set the variables to the previously set configuration. If we
		don't do this the variables will be always initialized to  the previous lines
		NOTE: Parenthesis are mandatory or it won't work!*/
		chkgroup1 = (cmdOptions.showDebugInfo ? 1 : 0) | (cmdOptions.showExtraDebugInfo ? 2 : 0) | (cmdOptions.manageSymbolicIndexing ? 4 : 0) | (cmdOptions.enableTracingAsTainted ? 8 : 0);
		chkgroup2 = (cmdOptions.automatedTainting ? 1 : 0) | (cmdOptions.taintEndOfString ? 2 : 0);
		chkgroup3 = (cmdOptions.taintArgv ? 1 : 0) | (cmdOptions.taintArgv0 ? 2 : 0) | (cmdOptions.taintArgc ? 4 : 0) | (cmdOptions.taintRecv ? 8 : 0) | (cmdOptions.taintFread ? 16 : 0);
		chkgroup4 = (cmdOptions.addCommentsControlledOperands ? 1 : 0) | (cmdOptions.RenameTaintedFunctionNames ? 2 : 0) | (cmdOptions.addCommentsSymbolicExpresions ? 4 : 0) | (cmdOptions.paintExecutedInstructions ? 8 : 0);

		symbolic_or_taint_engine = cmdOptions.use_symbolic_engine ? 0 : 1;
	}
	if (AskUsingForm_c(form,
		modcb, // the call to this function can be omitted. It's only usefull if a checkbox activate or dissable other elements of the form
		&cmdOptions.limitInstructionsBeforeAskingUser,
		&cmdOptions.limitTime,
		&cmdOptions.limitInstructionsTracingMode,
		&symbolic_or_taint_engine,
		&chkgroup1,
		&chkgroup2,
		&chkgroup3,
		&chkgroup4,
		&cmdOptions.color_tainted,
		&cmdOptions.color_tainted_condition,
		&cmdOptions.color_executed_instruction
		) > 0)
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
		cmdOptions.showDebugInfo = chkgroup1 & 1 ? 1 : 0;
		cmdOptions.showExtraDebugInfo = chkgroup1 & 2 ? 1 : 0;
		cmdOptions.manageSymbolicIndexing = chkgroup1 & 4 ? 1 : 0;
		cmdOptions.enableTracingAsTainted = chkgroup1 & 8 ? 1 : 0;
		//
		cmdOptions.automatedTainting = chkgroup2 & 1 ? 1 : 0;
		cmdOptions.taintEndOfString = chkgroup2 & 2 ? 1 : 0;
		//
		cmdOptions.taintArgv = chkgroup3 & 1 ? 1 : 0;
		cmdOptions.taintArgv0 = chkgroup3 & 2 ? 1 : 0;
		cmdOptions.taintArgc = chkgroup3 & 4 ? 1 : 0;
		cmdOptions.taintRecv = chkgroup3 & 8 ? 1 : 0;
		cmdOptions.taintFread = chkgroup3 & 16 ? 1 : 0;
		//
		cmdOptions.addCommentsControlledOperands = chkgroup4 & 1 ? 1 : 0;
		cmdOptions.RenameTaintedFunctionNames = chkgroup4 & 2 ? 1 : 0;
		cmdOptions.addCommentsSymbolicExpresions = chkgroup4 & 4 ? 1 : 0;
		cmdOptions.paintExecutedInstructions = chkgroup4 & 8 ? 1 : 0;


		if (cmdOptions.showDebugInfo){
			msg("\n"
				"limitInstructionsBeforeAskingUser: %lld\n"
				"limitTime: %lld\n"
				"limitInstructionsTracingMode: %lld\n"
				"use_symbolic_engine: %s\n"
				"showDebugInfo: %s\n"
				"showExtraDebugInfo: %s\n"
				"manageSymbolicIndexing: %s\n"
				"enableTracingAsTainted: %s\n"			
				"automatedTainting: %s\n"
				"taintEndOfString: %s\n"
				"taintArgv: %s\n"
				"taintArgv0: %s\n"
				"taintArgc: %s\n"
				"taintRecv: %s\n"
				"taintFread: %s\n"
				"addCommentsControlledOperands: %s\n"
				"RenameTaintedFunctionNames: %s\n"
				"addCommentsSymbolicExpresions: %s\n"
				"paintExecutedInstructions: %s\n"
				"color_tainted: %x\n"
				"color_tainted_condition: %x\n"
				"color_tainted_execution: %x\n",
				cmdOptions.limitInstructionsBeforeAskingUser,
				cmdOptions.limitTime,
				cmdOptions.limitInstructionsTracingMode,
				cmdOptions.use_symbolic_engine ? "symbolic engine enabled" : "tainting engine enabled",
				cmdOptions.showDebugInfo ? "true" : "false",
				cmdOptions.showExtraDebugInfo ? "true" : "false",
				cmdOptions.manageSymbolicIndexing ? "true" : "false",
				cmdOptions.enableTracingAsTainted ? "true" : "false",
				cmdOptions.automatedTainting ? "true" : "false",
				cmdOptions.taintEndOfString ? "true" : "false",
				cmdOptions.taintArgv ? "true" : "false",
				cmdOptions.taintArgv0 ? "true" : "false",
				cmdOptions.taintArgc ? "true" : "false",
				cmdOptions.taintRecv ? "true" : "false",
				cmdOptions.taintFread ? "true" : "false",
				cmdOptions.addCommentsControlledOperands ? "true" : "false",
				cmdOptions.RenameTaintedFunctionNames ? "true" : "false",
				cmdOptions.addCommentsSymbolicExpresions ? "true" : "false",
				cmdOptions.paintExecutedInstructions ? "true" : "false",
				cmdOptions.color_tainted,
				cmdOptions.color_tainted_condition,
				cmdOptions.color_executed_instruction
				);
		}
	}

}