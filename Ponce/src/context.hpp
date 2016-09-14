//! \file
/*
**  Copyright (c) 2016 - Ponce
**  Authors:
**         Alberto Garcia Illera		agarciaillera@gmail.com
**         Francisco Oca				francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

#pragma once
//Triton
#include <api.hpp>
//IDA
#include <pro.h> 

void needConcreteMemoryValue(triton::arch::MemoryAccess& mem);
void needConcreteRegisterValue(triton::arch::Register& reg);
triton::uint128 getCurrentMemoryValue(ea_t addr, triton::uint32 size);
triton::uint512 getCurrentRegisterValue(triton::arch::Register& reg);