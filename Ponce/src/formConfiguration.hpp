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
#ifdef __IDA70__
void idaapi btn_cb(TWidget *[], int);
#else
void idaapi btn_cb(TView *[], int);
#endif
void prompt_conf_window(void);

static const char form[] =
"STARTITEM 0\n"
"Ponce Configuration v" VERSION "\n\n"
"%/"
"<#time in seconds#Time limit before ask user (seconds)             :D1:10:10>\n"
"<#Number of the instructions executed during tracing before ask to the user#Limit the number of instructions in tracing mode :D2:10:10>\n"
"\n"
// Choser between using only the tainting engine or the symbolic one
"<#It will let you perform symbolic execution and track user controlled input#Select engine to use#Symbolic Engine:R3>\n"
"<#It allow you to track user controlled input#Taint Engine:R4>>\n"
//
"<#Ponce will be auto initialized with IDA#Auto init and debug#Auto init Ponce with IDA:C5>\n"
"<#Show debug info#Show debug info in the output windows:C6>\n"
"<#Max debug verbosity#Show EXTRA debug info in the output windows:C7>>\n"
//
"<#Auto taint argv. Will try to find the main function and taint#Taint/Symbolic options#Taint/Symbolize argv:C8>\n"
"<#Taint the null byte from a string to taint his lenght#- Taint/Symbolize end of string \\0:C9>\n"
"<#In some cases argv[0] may be used to perform some checks#- Taint/Symbolize argv[0]:C10>\n"
"<#Self explanatory#- Taint/Symbolize argc:C11>\n"
"<#Taint the buffer received in every recv API call#\tTaint/Symbolize recv (not implemented):C12>\n"
"<#Taint the buffer read from a file#Taint/Symbolize fread (not implemented):C13>\n"
"<#Enable or disable an optimzation to only generate expressions when the instr is tainted/symbolic#Enable optimization 'only generate expr on tainted/symbolic':C14>\n"
"<#Use 'smart' approaches to symbolic indexing problems#Manage symbolic memory index (not implemented):C15>>\n"
//
"<#Add comments to controlled operands#IDA View expand info#Add comments with controlled operands:C16>\n"
"<#This helps to track the tainted functions in large programms#Rename tainted function names:C17>\n"
"<#Will add a comment for every instruction with his symbolic expression. Will dirt the IDA view.#Add comments with symbolic expresions:C18>\n"
"<#Self explanatory#Paint executed instructions:C19>>\n"
//
"<#comment#Color Tainted Instruction     :K20:::>\n"
"<#comment#Color Executed Instruction    :K21:::>\n"
"<#comment#Color Tainted Condition       :K22:::>\n"

"<#Ponce will blacklist some functions that do not modify the passed parameters. You can select a file with the functions you want to blacklist#Blacklist file path:f23::40:>\n"

"\n"
;
