#pragma once

//Triton
#include <api.hpp>

void __cdecl needConcreteMemoryValue(triton::arch::MemoryAccess& mem);
void __cdecl needConcreteRegisterValue(triton::arch::Register& reg);