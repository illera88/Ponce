//! \file
/*
**  Copyright (c) 2020 - Ponce
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
int idaapi modcb(int fid, form_actions_t& fa)
{
    ushort isActivated = 0;
    switch (fid)
    {
    case -1: // called at the begining
        fa.get_checkbox_value(3, &isActivated); // get symbolic engine value
        fa.enable_field(8, isActivated ? 1 : 0);
        fa.enable_field(9, isActivated ? 1 : 0);
        fa.enable_field(12, isActivated ? 1 : 0);
        fa.enable_field(13, isActivated ? 1 : 0);
        fa.enable_field(14, !isActivated ? 1 : 0); // TAINT_THROUGH_POINTERS only when tainting engine
        break;
    case -2:
        break;
    case 3:
    case 4:
        fa.get_checkbox_value(3, &isActivated); // get symbolic engine value
        fa.enable_field(8, isActivated ? 1 : 0);
        fa.enable_field(9, isActivated ? 1 : 0);
        fa.enable_field(12, isActivated ? 1 : 0);
        fa.enable_field(13, isActivated ? 1 : 0);
        fa.enable_field(14, !isActivated ? 1 : 0); // TAINT_THROUGH_POINTERS only when tainting engine
        break;
    case 5:
        fa.get_checkbox_value(5, &isActivated);
        if (!isActivated)
            fa.set_checkbox_value(6, &isActivated);
        break;
    case 6:
        // if extra verbosity set then regular verbosity set too
        fa.get_checkbox_value(6, &isActivated);
        if(isActivated)
            fa.set_checkbox_value(5, &isActivated);
        break;
    default:
        break;
    }
    return 1;
}

/*IDA windows style is a FUMADA!!
If you want to create a new checkbox you should:
- add it to the form variable that its in formChoser.hpp
- Create the global variable in cmdOptions
- if you want to inizialize it to something do it in the first if of the function (if (!cmdOptions.already_configured))
- in the else you should add the correspondant item in the chkgroup where you are adding the item
- if AskUsingForm_c success (user clicks accept) set the value of your item from his correspondent chkgroupX
- add it to the msg at the end of the function for debug purposes*/
void prompt_conf_window(void) {
    /*We should create as many ushort variables as groups of checkboxes we have in the form window*/
    ushort chkgroup1, chkgroup2, chkgroup3;
    ushort symbolic_or_taint_engine = 0;

    if (!cmdOptions.already_configured) {
        //Here we can initialize the checkboxes by group. use 1 | 2 .. to select multiple 
        chkgroup1 = 1;
        chkgroup2 = 0;
        chkgroup3 = 1 | 2;

        cmdOptions.blacklist_path[0] = '\0'; // Will use this to check if the user set some path for the blacklist
    }
    else {
        /*By default all the variables are set to false. If the user wants to change the configuration
        after using the plugin we should set the variables to the previously set configuration. If we
        don't do this the variables will be always initialized to  the previous lines
        NOTE: Parenthesis are mandatory or it won't work!*/
        chkgroup1 = (cmdOptions.showDebugInfo ? 1 : 0) | (cmdOptions.showExtraDebugInfo ? 2 : 0);
        chkgroup2 = (cmdOptions.CONCRETIZE_UNDEFINED_REGISTERS ? 1 : 0) | (cmdOptions.CONSTANT_FOLDING ? 2 : 0) | (cmdOptions.SYMBOLIZE_INDEX_ROTATION ? 4 : 0) | (cmdOptions.AST_OPTIMIZATIONS ? 8 : 0) | (cmdOptions.TAINT_THROUGH_POINTERS ? 16 : 0);
        chkgroup3 = (cmdOptions.addCommentsControlledOperands ? 1 : 0) | (cmdOptions.RenameTaintedFunctionNames ? 2 : 0) | (cmdOptions.addCommentsSymbolicExpresions ? 4 : 0);

        symbolic_or_taint_engine = cmdOptions.use_symbolic_engine ? 0 : 1;
    }
    if (ask_form(form,
        modcb, // the call to this function can be omitted. It's only usefull if a checkbox activate or dissable other elements of the form
        &symbolic_or_taint_engine,
        &chkgroup1,
        &chkgroup2,
        &chkgroup3,
        &cmdOptions.limitTime,
        &cmdOptions.limitInstructionsTracingMode,
        &cmdOptions.solver_timeout,
        &cmdOptions.color_tainted,
        &cmdOptions.color_executed_instruction,
        &cmdOptions.color_tainted_condition,
        &cmdOptions.blacklist_path
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
        //
        cmdOptions.CONCRETIZE_UNDEFINED_REGISTERS = chkgroup2 & 1 ? 1 : 0;
        cmdOptions.CONSTANT_FOLDING = chkgroup2 & 2 ? 1 : 0;
        cmdOptions.SYMBOLIZE_INDEX_ROTATION = chkgroup2 & 4 ? 1 : 0;
        cmdOptions.AST_OPTIMIZATIONS = chkgroup2 & 8 ? 1 : 0;
        cmdOptions.TAINT_THROUGH_POINTERS = chkgroup2 & 16 ? 1 : 0;
        
        // Make sure that modes are correctly set since some engines
        // can't have some modes activated
        if (cmdOptions.use_symbolic_engine) {
            cmdOptions.TAINT_THROUGH_POINTERS = false;
        }
        else {
            cmdOptions.CONCRETIZE_UNDEFINED_REGISTERS = false;
            cmdOptions.CONSTANT_FOLDING = false;
            cmdOptions.SYMBOLIZE_INDEX_ROTATION = false;
            cmdOptions.AST_OPTIMIZATIONS = false;
        }

        cmdOptions.addCommentsControlledOperands = chkgroup3 & 1 ? 1 : 0;
        cmdOptions.RenameTaintedFunctionNames = chkgroup3 & 2 ? 1 : 0;
        cmdOptions.addCommentsSymbolicExpresions = chkgroup3 & 4 ? 1 : 0;

        if (cmdOptions.blacklist_path[0] != '\0') {
            //Means that the user set a path for custom blacklisted functions
            if (blacklkistedUserFunctions != NULL) {
                //Check if we had a previous custom blacklist and if so we delete it
                blacklkistedUserFunctions->clear();
                delete blacklkistedUserFunctions;
                blacklkistedUserFunctions = NULL;
            }
            readBlacklistfile(cmdOptions.blacklist_path);
        }

        save_options(&cmdOptions);
        if (cmdOptions.showDebugInfo) {
            msg("\n"
                "limitTime: %lld\n"
                "limitInstructionsTracingMode: %lld\n"
                "solver_timeout: %lld\n"
                "use_symbolic_engine: %s\n"
                "showDebugInfo: %s\n"
                "showExtraDebugInfo: %s\n"
                "CONCRETIZE_UNDEFINED_REGISTERS: %s\n"
                "CONSTANT_FOLDING: %s\n"
                "SYMBOLIZE_INDEX_ROTATION: %s\n"
                "AST_OPTIMIZATIONS: %s\n"
                "TAINT_THROUGH_POINTERS: %s\n"
                "addCommentsControlledOperands: %s\n"
                "RenameTaintedFunctionNames: %s\n"
                "addCommentssymbolizexpresions: %s\n"
                "addCommentssymbolizexpresions: %s\n"
                "color_tainted_execution: %x\n"
                "color_tainted_condition: %x\n",
                cmdOptions.limitTime,
                cmdOptions.limitInstructionsTracingMode,
                cmdOptions.solver_timeout,
                cmdOptions.use_symbolic_engine ? "symbolic engine enabled" : "tainting engine enabled",
                cmdOptions.showDebugInfo ? "true" : "false",
                cmdOptions.showExtraDebugInfo ? "true" : "false",
                cmdOptions.CONCRETIZE_UNDEFINED_REGISTERS ? "true" : "false",
                cmdOptions.CONSTANT_FOLDING ? "true" : "false",
                cmdOptions.SYMBOLIZE_INDEX_ROTATION ? "true" : "false",
                cmdOptions.AST_OPTIMIZATIONS ? "true" : "false",
                cmdOptions.TAINT_THROUGH_POINTERS ? "true" : "false",
                cmdOptions.addCommentsControlledOperands ? "true" : "false",
                cmdOptions.RenameTaintedFunctionNames ? "true" : "false",
                cmdOptions.addCommentsSymbolicExpresions ? "true" : "false",
                cmdOptions.color_tainted,
                cmdOptions.color_executed_instruction,
                cmdOptions.color_tainted_condition
            );
        }
    }
}