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
#include <triton/api.hpp>

#include <ida.hpp>
//A variable to store if the plugin has hooked the ui and debugger events
bool hooked = false;

//This struct stores all the global variables used for the current state of the Ponce plugin during execution
//The idea is restore this sctruct when we restore the snapshot
runtime_status_t ponce_runtime_status;

//Options
struct cmdOptionStruct cmdOptions;

//Snapshot object, defined in the snapshot.cpp
Snapshot snapshot = Snapshot();

//Used to point to the vector of blacklisted user functions
std::vector<std::string>* blacklkistedUserFunctions = nullptr;

triton::API api;

std::map<ea_t, struct instruction_info> ponce_comments;

bool hexrays_present = false;

ponce_table_chooser_t* ponce_table_chooser = nullptr;