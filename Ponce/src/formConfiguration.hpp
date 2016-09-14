#pragma once
//IDA
#include <ida.hpp>

int idaapi modcb(int fid, form_actions_t &fa);
void idaapi btn_cb(TView *[], int);

void prompt_conf_window(void);

static const char form[] =
"STARTITEM 0\n"
//"@0:477[]\n"
"Ponce Configuration\n\n"
"%/"
"<#time in seconds#Time limit before ask user\t                                  :L23:10:10>\n"
"<#number of instructions1#Limit the number of instructions in tracing mode\t            :L21:10:10>\n"
"\n"
// Choser between using only the tainting engine or the symbolic one
"<#It will let you perform symbolic execution and track user controlled input#Select engine to use#Symbolic Engine:R><#It allow you to track user controlled input#Taint Engine:R>>"
"\n"
//
"<#Show debug info#Debug options#"
"Show debug info in the output windows:C10>\n"
"<#Max debug verbosity#Show EXTRA debug info in the output windows:C16>\n"
"<#Use 'smart' approaches to symbolic indexing problems#Manage symbolic indexing:C11>\n"
"<#Pretty self explanatory#Enable tracing the first time something is tainted:C12>>\n"
//
"<#Try to automatically taint user input for you#Other pack#"
"Configure automated tainting:C18>\n"
"<#Taint the null byte from a string to taint his lenght#Taint end of string:C38>>\n"
//
"<#Auto taint argv. Will try to find the main function and taint#Tainting options#"
"Taint argv:C29>\n"
"<#In some cases argv[0] may be used to perform some checks#- Taint argv[0]:C19>\n"
"<#Self explanatory#- Taint argc:C24>\n"
"<#Taint the buffer received in every recv API call#\tTaint recv:C25>\n"
"<#Taint the buffer read from a file#Taint fread:C26>>\n"
//
"<#Add comments to controlled operands#IDA View expand info#"
"Add comments with controlled operands:C69>\n"
"<#This helps to track the tainted functions in large programms#Rename tainted function names:C89>\n"
"<#Will add a comment for every instruction with his symbolic expression. Will dirt the IDA view.#Add comments with symbolic expresions:C70>\n"
"<#Self explanatory#Paint executed instructions:C88>>\n"
//
"<#comment#Color Tainted                 :K40:::>\n"
"<#comment#Color Tainted Condition       :K41:::>\n"
"<#comment#Color Executed Instruction    :K42:::>\n"

"<#Ponce will blacklist some functions that do not modify the passed parameters. You can select a file with the functions you want to blacklist#Blacklist file path    :f43::40:>\n"

"\n"
;

