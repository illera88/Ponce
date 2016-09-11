#pragma once
//Ponce
#include "trigger.hpp"
//Triton
#include <api.hpp>

typedef struct PathConstraint{
	triton::__uint conditionRipId;
	triton::__uint conditionAddr;
	triton::__uint takenAddr;
	triton::__uint notTakenAddr;
	unsigned int bound;

	PathConstraint(triton::__uint conditionRipId, triton::__uint conditionAddr, triton::__uint takenAddr, triton::__uint notTakenAddr, unsigned int bound)
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
} runtime_status_t;

extern runtime_status_t ponce_runtime_status;