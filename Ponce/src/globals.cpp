/*This file containts all the global variables used around the plugin code*/
#include "trigger.hpp"

//This variable defines if there is a limit for the number of instructions traced by the plugin
bool limit_traced_instructions = true;
//This variable stores the number of instructions executed during the tracing
int total_number_traced_ins = 0;
//This variable is used to count how many instructions were executed after the user was asked
int current_trace_counter = 0;
//This is the max number of instructions executed during tracing before ask to the user if he wants to execute more
int max_traced_instructions = 20;
//A variable to store if the plugin has hooked the ui and debugger events
bool hooked = false;
//Is it something tainted?
bool is_something_tainted = false;
//Trigger to enable/disable triton
Trigger runtimeTrigger;
