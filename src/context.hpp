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
//Triton
#include <triton/api.hpp>
//IDA
#include <pro.h> 

void needConcreteMemoryValue_cb(triton::API& api, const triton::arch::MemoryAccess& mem);
void needConcreteRegisterValue_cb(triton::API& api, const triton::arch::Register& reg);
triton::uint512 IDA_getCurrentMemoryValue(ea_t addr, triton::uint32 size);
triton::uint512 IDA_getCurrentRegisterValue(const triton::arch::Register& reg);