//! \file
/*
**  Copyright (c) 2016 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

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

//Used to point to the vector of blacklisted user functions
std::vector<std::string> *blacklkistedUserFunctions = NULL;