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
#include "snapshot.hpp"
#include "runtime_status.hpp"
//IDA
#include <kernwin.hpp>
//Triton
#include <api.hpp>
//Version number
#include "../../VERSION_NUMBER"

#if !defined(__EA64__)
//#define X86_32 It is already defined in the preprocessor options
#define TRITON_X86_REG_XSP triton::arch::x86::x86_reg_esp
#define TRITON_REG_XIP triton::arch::x86::x86_reg_eip
#define TRITON_REG_ID_XAX triton::arch::x86::ID_REG_EAX
#define TRITON_X86_REG_XAX triton::arch::x86::x86_reg_eax
#define TRITON_ARCH triton::arch::ARCH_X86
#define REG_SIZE 4
#define ARCH arch::ARCH_X86
#define strtol_m strtol
#define HEX_FORMAT "%#x"
#define CPUSIZE 32

#else
//#define X86_64
#define TRITON_X86_REG_XSP triton::arch::x86::x86_reg_rsp
#define TRITON_REG_XIP triton::arch::x86::x86_reg_rip
#define TRITON_REG_ID_XAX triton::arch::x86::ID_REG_RAX
#define TRITON_X86_REG_XAX triton::arch::x86::x86_reg_rax
#define TRITON_ARCH triton::arch::ARCH_X86_64
#define ARCH arch::ARCH_X86_64
#define strtol_m strtoll
#define REG_SIZE 8
#define HEX_FORMAT "%#llx"
#define CPUSIZE 64
#endif

//stdcall does not exist in Linux so lets define it to nothing
#if defined(__LINUX__) || defined(__MAC__)
#define __stdcall
#endif

#define __END__ -1

//All the options:
#define SNAPSHOT_DESCRIPTION "Before use the script"
//It enables the instruction tracing the first time a register/memory is tainted
#define ENABLE_STEP_INTO_WHEN_TAINTING true

#define RENAME_TAINTED_FUNCTIONS_PREFIX "T_"
#define RENAME_TAINTED_FUNCTIONS_PATTERN RENAME_TAINTED_FUNCTIONS_PREFIX"%03d_"
#define RENAME_TAINTED_FUNCTIONS_PATTERN_LEN 6 

struct action{
	const action_desc_t* action_decs;
	int view_type[44]; // This are the posible views
	bool enable_taint;
	bool enable_symbolic;
	char const* menu_path; // this is where the item will be conteined in
};

extern Snapshot snapshot;

//All the global variables:
extern struct action action_list[];
extern bool hooked;

//We could use this if we want to keep all the instructions in memory
//extern std::map<ea_t, std::list<triton::arch::Instruction *>> instructions_executed_map;

//User options
struct cmdOptionStruct{
	bool auto_init = false;
	uint64 limitInstructionsTracingMode = 10000;
	uint64 limitTime = 60; //seconds

	//all this variables should be false and initialized in prompt_conf_window in utils.cpp
	bool already_configured = false; // We use this variable to know if the user already configured anything or if this is the first configuration promt
	
	//Only one out of the two will be selected
	bool  use_tainting_engine = false;
	bool  use_symbolic_engine = false;

	bgcolor_t color_tainted = 0x99FFCE;
	bgcolor_t color_tainted_condition = 0x00b377;
	bgcolor_t color_executed_instruction = 0xe6e6e6;

	bool showDebugInfo = false;
	bool showExtraDebugInfo = false;
	bool manageSymbolicIndexing = false;
	bool taintArgv = false;
	bool taintEndOfString = false;
	bool taintArgv0 = false;
	bool taintArgc = false;
	bool taintRecv = false;
	bool taintFread = false;
	bool only_on_optimization = true;

	bool addCommentsControlledOperands = false;
	bool RenameTaintedFunctionNames = false;
	bool addCommentsSymbolicExpresions = false;
	bool paintExecutedInstructions= false;

	char blacklist_path[QMAXPATH];
};
extern struct cmdOptionStruct cmdOptions;

class Input
{
public:
	int bound;
	std::vector <triton::arch::MemoryAccess> memOperand;
	std::vector <triton::arch::Register> regOperand;

	//! Constructor.
	Input()
	{
		this->bound = 0;
	}

	//! Destructor.
	~Input()
	{
	}
};

//This variable stores the ea where the popup menu is being displayed, this is use to add the submenus for Solve condition
extern ea_t popup_menu_ea;

extern std::vector<std::string> *blacklkistedUserFunctions; 

extern void idaapi term(void);