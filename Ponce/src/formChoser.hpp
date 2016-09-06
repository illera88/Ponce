
#pragma once
//IDA
#include <ida.hpp>

int idaapi modcb(int fid, form_actions_t &fa);
void idaapi btn_cb(TView *[], int);

static const char form[] =
"@0:477[]\n"
"Ponce Configuration\n"
"\n"
"%/Enter alternate string for the %9D operand\n"
"\n"
"  <~O~perand:A5:100:40::>\n"
"  <~X~:D4:100:10::>\n"
"  <~Y~:D3:100:10::>\n"
"  <~W~:D2:100:10::>\n"
"  <~H~:D1:100:10::>\n"
"\n"
"<#comment#Show debug info in the output windows:C10>\n"
"<#comment#Manage symbolic indexing:C11>\n"
"<#comment#Enable tracing the first time something is tainted:C12>\n"
"<#comment#Limit the number of instructions in tracing mode:C13><#number of instructions#N:D21::>\n"
//"<~L~ength:D1:100:10::>\n"
//"<Number:D4:100:10::>\n"
"<#comment#Limit of instructions to execute before ask to the user:C14><#number of instructions#N:D22::>\n"
"<#comment#Time limit before ask user:C15><#time in seconds#Time:D23::>\n"
"<#comment#Only use tainting mode:C16>\n"
"<#comment#Automatically rename the functions with tainted instructions/conditions :C17>\n"
"<#comment#Configure automated tainting:C18>\n"
"<#comment#Taint argv[0]:C19>\n"
"<#comment#Taint argc:C20>>\n"
"\n"


;

