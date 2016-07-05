#pragma once

#include "trigger.hpp"

//IDA
#include <kernwin.hpp>

//DR required headers
//#ifdef _WIN32
//#define WINDOWS
//#elif __APPLE__
//#define MACOS
//#else 
//#define LINUX
//#endif

#if defined(__i386) || defined(_M_IX86)
//#define X86_32
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

#define COLOR_TAINTED 0xc7c7ff
#define COLOR_TAINTED_JUMP 0xc9c7ff

extern int g_nb_insn;
extern const int g_max_insn;
extern bool hooked;

struct action{
	char* name;
	char* label;
	const action_desc_t* callback;
	int view_type[44]; // This are the posible views
};

extern struct action action_list[];
extern bool is_something_tainted;
extern Trigger runtimeTrigger;


