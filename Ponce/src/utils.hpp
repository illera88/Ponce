#pragma once
#include <string>
//Triton
#include <api.hpp>

void start_tainting_or_symbolic_analysis();
triton::arch::Register *str_to_register(std::string register_name);
void taint_all_memory(triton::__uint address, triton::__uint size);
void symbolize_all_memory(triton::__uint address, triton::__uint size, char* comment);
int ask_for_a_snapshot();
bool already_exits_a_snapshot();
ea_t find_function(char *function_name);
triton::__uint get_args(int argument_number, bool skip_ret);
triton::__uint get_args_pointer(int argument_number, bool skip_ret);
char read_char_from_ida(ea_t address);
triton::__uint read_uint_from_ida(ea_t address);
void rename_tainted_function(ea_t address);
void add_symbolic_expressions(triton::arch::Instruction* tritonInst, ea_t address);