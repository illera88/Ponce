#pragma once

#include "trigger.hpp"

//IDA
#include <kernwin.hpp>

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

#define COLOR_TAINTED 0x99FFCE
#define COLOR_TAINTED_CONDITION 0x00b377
#define SNAPSHOT_DESCRIPTION "Before use the script"
#define DEBUG true
#define EXTRADEBUG false
//It enables the instruction tracing the first time a register/memory is tainted
#define ENABLE_TRACING_WHEN_TAINTING true
//It runs the plugin when it is initiallized
#define AUTO_RUN true
#define ENABLE_TAINTING_ENGINE true
#define ENABLE_SYMBOLIC_ENGINE true
#define TAINT_ARGV true
#define TAINT_ARGC true
#define SKIP_ARGV0 true
#define TAINT_END_OF_STRING false

struct action{
	char* name;
	char* label;
	const action_desc_t* callback;
	int view_type[44]; // This are the posible views
};

extern struct action action_list[];
extern bool limit_traced_instructions;
extern int total_number_traced_ins;
extern int current_trace_counter;
extern int max_traced_instructions;
extern bool hooked;
extern bool is_something_tainted;
extern Trigger runtimeTrigger;