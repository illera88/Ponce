//! \file
/*
**  Copyright (c) 2020 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

#include <iostream>
#include <fstream>

// Ponce
#include "blacklist.hpp"
#include "globals.hpp"
#include "callbacks.hpp"
#include "utils.hpp"
#include "triton_logic.hpp"

// IDA
#include <ida.hpp>
#include <dbg.hpp>
#include <loader.hpp>
#include <intel.hpp>
#include <bytes.hpp>

std::list<breakpoint_pending_action> breakpoint_pending_actions;

std::vector<std::string> builtin_black_functions = {
    "printf",
    "puts",
    "putc",
    "sleep",
    "recv",
    "recvfrom",
    "send",
    "closesocket",
    "exit",
    "abort",
    "malloc",
    "calloc",
    "realloc",
    "free",
    "calloc_crt",
    "realloc_crt",

    //cstdio
    "clearerr",
    "fclose",
    "feof",
    "ferror",
    "fflush",
    "fgetc",
    "fgetpos",
    "fgets",
    "fopen",
    "fprintf",
    "fputc",
    "fputs",
    "fread",
    "freopen",
    "fscanf",
    "fseek",
    "fsetpos",
    "ftell",
    "fwrite",
    "getc",
    "getchar",
    "gets",
    "perror",
    "printf",
    "putc",
    "putchar",
    "puts",
    "remove",
    "rename",
    "rewind",
    "scanf",
    "setbuf",
    "setvbuf",
    "snprintf",
    "sprintf",
    "sscanf",
    "tmpfile",
    "tmpnam",
    "ungetc",
    "vfprintf",
    "vfscanf",
    "vprintf",
    "vscanf",
    "vsnprintf",
    "vsprintf",
    "vsscanf",

    //Windows
    "Sleep",
    "HeapAlloc",
    "HeapFree",
    "HeapRealloc",
    "CreateThread",
    "NtTerminateProcess",
    "ExitThread",
    "TerminateThread",
    "ExitProcess",

    "EnterCriticalSection",
    "LeaveCriticalSection",
    "SendMessage",
    "WSAGetLastError",
    "WaitForSingleObject",
    "CloseHandle",
};

//Helper to concretize and untaint volatile registers
void concretizeAndUntaintVolatileRegisters()
{
    //ToDo: check how different compilers behave regarding volatile registers
#if defined(__i386) || defined(_M_IX86)
    char const* volatile_regs[] = { "eax", "ecx", "edx" };
#elif defined(__x86_64__) || defined(_M_X64)
    char const* volatile_regs[] = { "rax", "rcx", "rdx", "r8", "r9", "r10", "r11", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4", "ymm5" };
#endif

    for (const auto& [reg_id, reg] : tritonCtx.getAllRegisters())
    {
        for (size_t i = 0; i < sizeof(volatile_regs) / sizeof(char*); i++) {
            if (strcmp(reg.getName().c_str(), volatile_regs[i]) == 0) {
                tritonCtx.concretizeRegister(reg);
                tritonCtx.untaintRegister(reg);
            }
        }
    }
}

//Helper to concretize and untaint all registers
void concretizeAndUntaintAllRegisters()
{
    tritonCtx.concretizeAllRegister();
    //We untaint all the registers
    auto regs = tritonCtx.getAllRegisters();
    for (auto it = regs.begin(); it != regs.end(); it++)
    {
        tritonCtx.untaintRegister(it->second);
    }
}

/* We use this function to enable the trigger after a blacklisted function.
We are also concretizing all the registers. The idea is after call a external function
you couldn't assume any register has been unchanged, so we concretize all of them.*/
void enableTrigger_and_concretize_registers(ea_t main_address)
{
    ponce_runtime_status.runtimeTrigger.enable();
    concretizeAndUntaintVolatileRegisters();
}

void readBlacklistfile(char* path) {
    std::ifstream file(path);
    std::string str;
    blacklkistedUserFunctions = new std::vector<std::string>();
    while (std::getline(file, str)) {
        if (cmdOptions.showDebugInfo)
            msg("[+] Adding %s to the blacklist funtion list\n", str.c_str());
        blacklkistedUserFunctions->push_back(str);
    }
}


bool should_blacklist(ea_t pc, thid_t tid) {
    insn_t cmd;
    decode_insn(&cmd, pc);

    // We do this to blacklist API that does not change the tainted input
    if (cmd.itype == NN_call || cmd.itype == NN_callfi || cmd.itype == NN_callni)
    {
        //qstring callee = get_callee_name(pc);
        qstring callee;
        auto callee_lenght = get_func_name(&callee, pc);
        std::vector<std::string>* to_use_blacklist;

        //Let's check if the user provided any blacklist file or we sholuld use the built in one
        if (blacklkistedUserFunctions != nullptr) {
            to_use_blacklist = blacklkistedUserFunctions;
        }
        else {//We need to use the built in one
            to_use_blacklist = &builtin_black_functions;
        }

        for (const auto& blacklisted_function : *to_use_blacklist) {
            if (strcmp(callee.c_str(), blacklisted_function.c_str()) == 0)
            {
                //We are in a call to a blacklisted function.
                /*We should set a BP in the next instruction right after the
                blacklisted callback to enable tracing again*/
                ea_t next_ea = next_head(pc, BADADDR);
                add_bpt(next_ea, 1, BPT_EXEC);
                //We set a comment so the user know why there is a new bp there
                ponce_set_cmt(next_ea, "Temporal bp set by ponce for blacklisting\n", false);

                breakpoint_pending_action bpa;
                bpa.address = next_ea;
                bpa.ignore_breakpoint = false;
                bpa.callback = enableTrigger_and_concretize_registers; // We will enable back the trigger when this bp get's reached

                //We add the action to the list
                breakpoint_pending_actions.push_back(bpa);

                //Disabling step tracing...
                disable_step_trace();

                //We want to tritonize the call, so the memory write for the ret address in the stack will be restore by the snapshot
                tritonize(pc, tid);
                ponce_runtime_status.runtimeTrigger.disable();

                return true;
            }
        }
    }
    return false;
}