#pragma once
//Triton
#include <api.hpp>

void __stdcall needConcreteMemoryValue(triton::arch::MemoryAccess& mem);
void __stdcall needConcreteRegisterValue(triton::arch::Register& reg);
triton::uint128 getCurrentMemoryValue(triton::__uint addr, triton::uint32 size);
triton::uint512 getCurrentRegisterValue(triton::arch::Register& reg);