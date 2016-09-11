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
	//Should we remove this item from the breakpoint_pending_actions list?
	bool removeMe;
} breakpoint_pending_action;

extern std::list<breakpoint_pending_action> breakpoint_pending_actions;

int idaapi tracer_callback(void * /*user_data*/, int notification_code, va_list va);
int idaapi ui_callback(void * /*ud*/, int notification_code, va_list va);
void tritonize(va_list va);
void reanalize_current_instruction();
void set_SMT_results(Input* input_ptr);