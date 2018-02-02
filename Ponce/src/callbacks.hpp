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

#include <idp.hpp>
#include <list>

#include "globals.hpp"

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

#ifdef __IDA70__
ssize_t idaapi tracer_callback(void * /*user_data*/, int notification_code, va_list va);
ssize_t idaapi ui_callback(void * /*ud*/, int notification_code, va_list va);
#else
int idaapi tracer_callback(void * /*user_data*/, int notification_code, va_list va);
int idaapi ui_callback(void * /*ud*/, int notification_code, va_list va);
#endif
void tritonize(va_list va);
void reanalize_current_instruction();
void set_SMT_results(Input* input_ptr);