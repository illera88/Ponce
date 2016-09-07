
#pragma once
//IDA
#include <ida.hpp>

int idaapi modcb(int fid, form_actions_t &fa);
void idaapi btn_cb(TView *[], int);

static const char form[] =
"STARTITEM 0\n"
//"@0:477[]\n"
"Ponce Configuration\n\n"
"%/"
"<#number of instructions2#Limit of instructions to execute before ask to the user\t     :L22:10:10>\n"
"<#time in seconds#Time limit before ask user\t                                  :L23:10:10>\n"
"<#number of instructions1#Limit the number of instructions in tracing mode\t            :L21:10:10>\n"
"\n"
"<#comment#Debug options#Show debug info in the output windows:C10>\n"
"<#comment#Manage symbolic indexing:C11>\n"
"<#comment#Enable tracing the first time something is tainted:C12>>\n"
//
"<#comment#Other pack#Only use tainting mode:C16>\n"
"<#comment#Automatically rename the functions with tainted instructions/conditions:C17>\n"
"<#comment#Configure automated tainting:C18>>\n"
//
"<#item hint#Tainting options#Taint argv[0]:C19>\n"
"<#comment#Taint argc:C24>\n"
"<#comment#Taint recv:C25>\n"
"<#comment#Taint fread:C26>>\n"
"\n"


;

