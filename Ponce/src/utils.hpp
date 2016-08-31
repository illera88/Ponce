#pragma once
#include <api.hpp>
#include <string>

triton::arch::Register *str_to_register(std::string register_name);
void taint_all_memory(triton::__uint address, triton::__uint size);