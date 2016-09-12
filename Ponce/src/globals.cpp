/*This file containts all the global variables used around the plugin code*/
#include <map>
//Ponce
#include "globals.hpp"
#include "trigger.hpp"
//Triton
#include "api.hpp"

//A variable to store if the plugin has hooked the ui and debugger events
bool hooked = false;

//This struct stores all the global variables used for the current state of the Ponce plugin during execution
//The idea is restore this sctruct when we restore the snapshot
runtime_status_t ponce_runtime_status;

//Options
struct cmdOptionStruct cmdOptions;

//Snapshot object, defined in the snapshot.cpp
Snapshot snapshot = Snapshot();

//This map stores all the executed instructions, the key is the address, and the value is a list of TritonInstructions. 
//One instruction could be executed more than one time
//std::map<ea_t, std::list<triton::arch::Instruction *>> instructions_executed_map;

ea_t popup_menu_ea;