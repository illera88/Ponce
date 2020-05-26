//! \file
/*
**  Copyright (c) 2020 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

#pragma once
#include <string>
//Triton
#include <triton/api.hpp>
//Ponce
#include "globals.hpp"

const triton::arch::register_e str_to_register(const qstring& register_name);
ea_t find_function(char const* function_name);
ea_t get_args(int argument_number, bool skip_ret);
ea_t get_args_pointer(int argument_number, bool skip_ret);
char read_char_from_ida(ea_t address);
ea_t read_regSize_from_ida(ea_t address);
void rename_tainted_function(ea_t address);
void add_symbolic_expressions(triton::arch::Instruction* tritonInst, ea_t address);
std::string notification_code_to_string(int notification_code);
bool load_options(struct cmdOptionStruct* cmdOptions);
bool save_options(struct cmdOptionStruct* cmdOptions);
bool ask_for_execute_native();
qstring get_callee_name(ea_t address);
void concretizeAndUntaintAllRegisters();
void enableTrigger_and_concretize_registers(ea_t main_address);
void readBlacklistfile(char* path);
std::uint64_t GetTimeMs64(void);
void concretizeAndUntaintVolatileRegisters();
short read_unicode_char_from_ida(ea_t address);
ea_t current_instruction();
void delete_ponce_comments();
bool ponce_set_cmt(ea_t ea, const char* comm, bool rptble, bool snapshot = false);
void ponce_set_item_color(ea_t ea, bgcolor_t color);
void comment_controlled_operands(triton::arch::Instruction* tritonInst, ea_t pc);