/*This file containts all the global variables used around the plugin code*/
#include <map>
//Ponce
#include "globals.hpp"
#include "trigger.hpp"
//Triton
#include "api.hpp"

//This variable defines if there is a limit for the number of instructions traced by the plugin
//bool limit_traced_instructions = true;
//This variable stores the number of instructions executed during the tracing
unsigned int total_number_traced_ins = 0;
//This variable is used to count how many instructions were executed after the user was asked
unsigned int current_trace_counter = 0;
//This is the max number of instructions executed during tracing before ask to the user if he wants to execute more
//unsigned int max_traced_instructions = 100;
//A variable to store if the plugin has hooked the ui and debugger events
bool hooked = false;
//Is it something tainted or symbolize?
bool is_something_tainted_or_symbolize = false;
//This index is used when we are renaming the tainted funcitons, to know the index
unsigned int tainted_functions_index = 0;
//Trigger to enable/disable triton
Trigger runtimeTrigger;
//This is the last instruction executed by triton, we need to reference to reanalize if the user taint a register
triton::arch::Instruction* last_triton_instruction;
//This variable is use to automatically do step into after a previous step_into, and in this way analyze a instruction by instruction a program
bool automatically_continue_after_step = false;
//This map stores all the executed instructions, the key is the address, and the value is a list of TritonInstructions. 
//One instruction could be executed more than one time
//std::map<triton::__uint, std::list<triton::arch::Instruction *>> instructions_executed_map;
//This variable stores all the path constraints executed before reach the current instruction
std::vector<PathConstraint> myPathConstraints;

//Snapshot object
Snapshot snapshot = Snapshot();