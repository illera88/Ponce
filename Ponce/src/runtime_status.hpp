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
//Ponce
#include "trigger.hpp"
//Triton
#include <api.hpp>
//IDA
#include <pro.h> 
#include <idd.hpp>

typedef struct PathConstraint{
	triton::usize conditionRipId;
	ea_t conditionAddr;
	ea_t takenAddr;
	ea_t notTakenAddr;
	unsigned int bound;

	PathConstraint(triton::usize conditionRipId, ea_t conditionAddr, ea_t takenAddr, ea_t notTakenAddr, unsigned int bound)
	{
		this->conditionRipId = conditionRipId;
		this->conditionAddr = conditionAddr;
		this->takenAddr = takenAddr;
		this->notTakenAddr = notTakenAddr;
		this->bound = bound;
	}
} PathConstraint;

//This struct stores all the global variables used for the current state of the Ponce plugin during execution
//The idea is restore this sctruct when we restore the snapshot
typedef struct runtime_status_t
{
	//This variable stores the number of instructions executed during the tracing
	unsigned int total_number_traced_ins;
	//This variable is use to have statistics
	unsigned int total_number_symbolic_ins;
	//This variable is use to have statistics
	unsigned int total_number_symbolic_conditions;
	//This variable is used to count how many instructions were executed after the user was asked
	unsigned int current_trace_counter;
	//Is it something tainted or symbolize?
	bool is_something_tainted_or_symbolize;
	//This index is used when we are renaming the tainted funcitons, to know the index
	unsigned int tainted_functions_index;
	//Trigger to enable/disable triton
	Trigger runtimeTrigger;
	//This is the last instruction executed by triton, we need to reference to reanalize if the user taint a register
	triton::arch::Instruction* last_triton_instruction;
	//This variable stores all the path constraints executed before reach the current instruction
	std::vector<PathConstraint> myPathConstraints;
	//This variable is used to know how much time the tracing was working, and stop if this time is bigger than the user defined value
	std::uint64_t tracing_start_time = 0;
	thid_t analyzed_thread;
	//Flag used to skip the step over done to deal with wow64 switching
	bool ignore_wow64_switching_step = false;
} runtime_status_t;

extern runtime_status_t ponce_runtime_status;