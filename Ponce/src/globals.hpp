#pragma once
//Ponce
#include "trigger.hpp"
//IDA
#include <kernwin.hpp>
//Triton
#include <api.hpp>

#if defined(__i386) || defined(_M_IX86)
//#define X86_32 It is already defined in the preprocessor options
#define TRITON_X86_REG_XSP triton::arch::x86::x86_reg_esp
#define TRITON_REG_XIP triton::arch::x86::x86_reg_eip
#define TRITON_REG_ID_XAX triton::arch::x86::ID_REG_EAX
#define TRITON_X86_REG_XAX triton::arch::x86::x86_reg_eax
#define REG_SIZE 4
#define ARCH arch::ARCH_X86
#define strtol_m strtol
#define HEX_FORMAT "%#x"
#define CPUSIZE 32

#elif defined(__x86_64__) || defined(_M_X64)
//#define X86_64
#define TRITON_X86_REG_XSP triton::arch::x86::x86_reg_rsp
#define TRITON_REG_XIP triton::arch::x86::x86_reg_rip
#define TRITON_REG_ID_XAX triton::arch::x86::ID_REG_RAX
#define TRITON_X86_REG_XAX triton::arch::x86::x86_reg_rax
#define ARCH arch::ARCH_X86_64
#define strtol_m strtoll
#define REG_SIZE 8
#define HEX_FORMAT "%#llX"
#define CPUSIZE 64
#endif

//All the options:
#define COLOR_TAINTED 0x99FFCE
#define COLOR_TAINTED_CONDITION 0x00b377
#define COLOR_EXECUTED_INSTRUCTION 0xe6e6e6
#define SNAPSHOT_DESCRIPTION "Before use the script"
#define DEBUG true
#define EXTRADEBUG true
//It enables the instruction tracing the first time a register/memory is tainted
#define ENABLE_STEP_INTO_WHEN_TAINTING true
#define PAINT_EXECUTED_INSTRUCTIONS true
//It runs the plugin when it is initiallized
#define AUTO_RUN true
//The two different modes, only one can be activated at one time
#define TAINT 0
#define SYMBOLIC 1
//This is the current mode
#define MODE SYMBOLIC
#define ENABLE_SYMBOLIC_ENGINE true
#define TAINT_ARGV true
#define TAINT_ARGC true
#define SKIP_ARGV0 true
#define TAINT_END_OF_STRING false
#define ADD_COMMENTS_WITH_CONTROLLED_OPERAND true
#define RENAME_TAINTED_FUNCTIONS false
#define RENAME_TAINTED_FUNCTIONS_PREFIX "T%03d_"
#define ADD_COMMENTS_WITH_SYMBOLIC_EXPRESSIONS false



struct action{
	char* name;
	char* label;
	const action_desc_t* callback;
	int view_type[44]; // This are the posible views
};

//All the global variables:
extern struct action action_list[];
extern bool limit_traced_instructions;
extern unsigned int total_number_traced_ins;
extern unsigned int current_trace_counter;
extern unsigned int max_traced_instructions;
extern bool hooked;
extern bool is_something_tainted_or_symbolize;
extern unsigned int tainted_functions_index;
extern Trigger runtimeTrigger;
extern triton::arch::Instruction* last_triton_instruction;
extern bool automatically_continue_after_step;
//We could use this if we want to keep all the instructions in memory
//extern std::map<triton::__uint, std::list<triton::arch::Instruction *>> instructions_executed_map;

//User options
struct cmdOptionStruct{
	bool showDebugInfo = false;
	bool manageSymbolicIndexing = false;
	bool enableTracingAsTainted = false;
	bool limitInstructionsTracingMode = false;
	bool limitInstructionsBeforeAskingUser = false;
	bool limitTime = false;
	bool onlyTainting = false;
	bool RenameFunctionNames = false;
	bool automatedTainting = false;
	bool taintArgv0 = false;
	bool taintArgc = false;	
};
extern struct cmdOptionStruct cmdOptions;

struct PathConstraint{
	triton::__uint conditionRipId;
	triton::__uint conditionAddr;
	triton::__uint takenAddr;
	triton::__uint notTakenAddr;

	PathConstraint(triton::__uint conditionRipId, triton::__uint conditionAddr, triton::__uint takenAddr, triton::__uint notTakenAddr)
	{
		this->conditionRipId = conditionRipId;
		this->conditionAddr = conditionAddr;
		this->takenAddr = takenAddr;
		this->notTakenAddr = notTakenAddr;
	}
};
extern std::vector<PathConstraint> myPathConstraints;
