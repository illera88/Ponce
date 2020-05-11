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
#include <triton/api.hpp>
//Version number
#include "../VERSION_NUMBER"

#define strtol_m strtoll

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

//struct IDA_actions {   
//    const action_desc_t* action_decs;
//    int view_type[10]; // This are the posible views
//    char const* menu_path; // this is where the item will be conteined in
//};

extern Snapshot snapshot;

//All the global variables:
//extern struct IDA_actions action_list[];
extern bool hooked;

//User options
struct cmdOptionStruct {
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

    bool AST_OPTIMIZATIONS = false;
    bool CONCRETIZE_UNDEFINED_REGISTERS = false;
    bool CONSTANT_FOLDING = false;
    bool SYMBOLIZE_INDEX_ROTATION = false;
    bool TAINT_THROUGH_POINTERS = false;

    bool addCommentsControlledOperands = false;
    bool RenameTaintedFunctionNames = false;
    bool addCommentssymbolizexpresions = false;
    bool paintExecutedInstructions = false;

    char blacklist_path[QMAXPATH];
};
extern struct cmdOptionStruct cmdOptions;

extern std::vector<std::string>* blacklkistedUserFunctions;

extern void idaapi term(void);

extern triton::API api;

extern std::list<std::pair<ea_t, int>> ponce_comments;

/* For backwards compatibility with IDA SDKs < 7.3 */
#if IDA_SDK_VERSION < 730
#define inf_get_min_ea()        inf.min_ea
#define inf_is_64bit()          inf.is_64bit()
#define inf_is_32bit()          inf.is_32bit()
#define WOPN_DP_TAB             WOPN_TAB
#endif

#ifdef __EA64__
#define MEM_FORMAT "%#" PRIx64
#define REG_XIP api.registers.x86_rip
#else
#define MEM_FORMAT "%#" PRIx32
#define REG_XIP api.registers.x86_eip
#endif // __EA64__
