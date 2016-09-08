
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
"<#number of instructions2#Limit of instructions to execute before ask to the user\t     :L22:10:10>\n"
"<#time in seconds#Time limit before ask user\t                                  :L23:10:10>\n"
"<#number of instructions1#Limit the number of instructions in tracing mode\t            :L21:10:10>\n"
"\n"
// Choser between using only the tainting engine or the symbolic one
"<#Comment1#Select engine to use#Symbolic Engine:R><Taint Engine:R>>"
"\n"
//
"<#comment#Debug options#"
"Show debug info in the output windows:C10>\n"
"<#comment#Show EXTRA debug info in the output windows:C16>\n"
"<#comment#Manage symbolic indexing:C11>\n"
"<#comment#Enable tracing the first time something is tainted:C12>>\n"
//
"<#comment#Other pack#"
"Configure automated tainting:C18>\n"
"<#comment#Taint end of string:C38>>\n"
//
"<#item hint#Tainting options#"
"Taint argv:C29>\n"
"<#comment#- Taint argv[0]:C19>\n"
"<#comment#- Taint argc:C24>\n"
"<#comment#\tTaint recv:C25>\n"
"<#comment#Taint fread:C26>>\n"
//
"<#item hint#IDA View expand info#"
"Add comments with controlled operands:C69>\n"
"<#comment#Rename tainted function names:C89>\n"
"<#comment#Add comments with symbolic expresions:C70>\n"
"<#comment#Paint executed instructions:C88>>\n"
//
"<#comment#Color Tainted                 :K40:::>\n"
"<#comment#Color Tainted Condition       :K41:::>\n"
"<#comment#Color Executed Instruction    :K42:::>\n"

"\n"
;

