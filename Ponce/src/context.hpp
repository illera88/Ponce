#pragma once
//Triton
#include <api.hpp>
//IDA
#include <pro.h> 

void needConcreteMemoryValue(triton::arch::MemoryAccess& mem);
void needConcreteRegisterValue(triton::arch::Register& reg);
triton::uint128 getCurrentMemoryValue(ea_t addr, triton::uint32 size);
triton::uint512 getCurrentRegisterValue(triton::arch::Register& reg);