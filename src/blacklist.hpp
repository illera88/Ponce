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
#include <vector>
#include <string>
#include <list>

#include <ida.hpp>
#include <idd.hpp>

//This struct is used to define a pending action when a breakpoint is triggered
typedef struct
{
    //The address where the breakpoint was set
    ea_t address;
    //If we found a previous breakpoint in the same address we should ignore it
    bool ignore_breakpoint;
    //This is the callback will be executed when this breakpoint is reached
    void(*callback)(ea_t);
} breakpoint_pending_action;

extern std::list<breakpoint_pending_action> breakpoint_pending_actions;


bool should_blacklist(ea_t pc, thid_t tid = 0);