//! \file
/*
**  Copyright (c) 2016 - Ponce
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

int idaapi modcb(int fid, form_actions_t &fa);
void idaapi btn_cb(TView *[], int);

void prompt_conf_window(void);

static const char form[] =
"STARTITEM 0\n"
"Ponce Configuration v"VERSION"\n\n"
"%/"
"<#time in seconds#Time limit before ask user (seconds)             :D1:10:10>\n"
"<#Number of the instructions executed during tracing before ask to the user#Limit the number of instructions in tracing mode :D2:10:10>\n"
"\n"
// Choser between using only the tainting engine or the symbolic one
"<#It will let you perform symbolic execution and track user controlled input#Select engine to use#Symbolic Engine:R3>\n"
"<#It allow you to track user controlled input#Taint Engine:R4>>\n"
//
"<#Show debug info#Debug options#Show debug info in the output windows:C5>\n"
"<#Max debug verbosity#Show EXTRA debug info in the output windows:C6>>\n"
//
"<#Auto taint argv. Will try to find the main function and taint#Taint/Symbolic options#Taint/Symbolize argv:C7>\n"
"<#Taint the null byte from a string to taint his lenght#- Taint/Symbolize end of string \\0:C8>\n"
"<#In some cases argv[0] may be used to perform some checks#- Taint/Symbolize argv[0]:C9>\n"
"<#Self explanatory#- Taint/Symbolize argc:C10>\n"
"<#Taint the buffer received in every recv API call#\tTaint/Symbolize recv (not implemented):C11>\n"
"<#Taint the buffer read from a file#Taint/Symbolize fread (not implemented):C12>\n"
"<#Enable or disable an optimzation to only generate expressions when the instr is tainted/symbolic#Enable optimization 'only generate expr on tainted/symbolic':C13>\n"
"<#Use 'smart' approaches to symbolic indexing problems#Manage symbolic memory index (not implemented):C14>>\n"
//
"<#Add comments to controlled operands#IDA View expand info#Add comments with controlled operands:C15>\n"
"<#This helps to track the tainted functions in large programms#Rename tainted function names:C16>\n"
"<#Will add a comment for every instruction with his symbolic expression. Will dirt the IDA view.#Add comments with symbolic expresions:C17>\n"
"<#Self explanatory#Paint executed instructions:C18>>\n"
//
"<#comment#Color Tainted Instruction     :K19:::>\n"
"<#comment#Color Executed Instruction    :K20:::>\n"
"<#comment#Color Tainted Condition       :K21:::>\n"

"<#Ponce will blacklist some functions that do not modify the passed parameters. You can select a file with the functions you want to blacklist#Blacklist file path:f22::40:>\n"

"\n"
;

