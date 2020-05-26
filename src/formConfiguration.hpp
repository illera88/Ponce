//! \file
/*
**  Copyright (c) 2020 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

#pragma once
//IDA
#include <ida.hpp>
#include "globals.hpp"

int idaapi modcb(int fid, form_actions_t& fa);
void idaapi btn_cb(TWidget* [], int);
void prompt_conf_window(void);

static const char form[] =
"STARTITEM 0\n"
"Ponce Configuration\n\n"
"%/"

// Choser between using only the tainting engine or the symbolic one
"<#It will let you perform symbolic execution and track user controlled input#Select engine to use#Symbolic Engine:R3>\n"
"<#It allow you to track user controlled input#Taint Engine:R4>>\n"
//
"<#Show debug info#Verbosity#Show Ponce debug info:C5>\n"
"<#Max debug verbosity#Show EXTRA Ponce debug info:C6>>\n"
//
"<#Concretize every registers tagged as undefined#Optimizations#CONCRETIZE_UNDEFINED_REGISTERS:C8>\n"
"<#Perform a constant folding optimization of sub ASTs which do not contain symbolic variables#CONSTANT_FOLDING:C9>\n"
"<#Symbolize index rotation for bvrol and bvror. This mode increases the complexity of solving#SYMBOLIZE_INDEX_ROTATION:C12>\n"
"<#Classical arithmetic optimisations to reduce the depth of the trees#AST_OPTIMIZATIONS:C13>\n"
"<#Spread the taint if an index pointer is already tainted#TAINT_THROUGH_POINTERS:C14>>\n"
//
"<#Add comments to controlled operands#IDA View expand info#Add comments with controlled operands:C15>\n"
"<#This helps to track the tainted functions in large programms#Add prefix to tainted function names:C16>\n"
"<#Will add a comment for every instruction with his symbolic expression. Will dirt the IDA view.#Add comments with symbolic expresions:C17>>\n"
"\n"
"Ponce will heads up you after:\n"
"<#Time in seconds#Seconds running               :D1:12:12>\n"
"<#Number of the instructions executed during tracing before ask to the user#Instructions executed         :D2:12:12>\n"
"\n"
"<#-1 is default colour#Color Tainted Instruction     :K19:::>\n"
"<#-1 is default colour#Color Executed Instruction    :K20:::>\n"
"<#-1 is default colour#Color Tainted Condition       :K21:::>\n"
"\n"
"<#Ponce will blacklist some functions that do not modify the passed parameters. You can select a file with the functions you want to blacklist#Function blacklist file path:f22::18:>\n"

"\n"
;
