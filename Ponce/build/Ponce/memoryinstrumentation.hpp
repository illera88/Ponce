
#pragma once

//DR
#include <dr_api.h>
//Triton
#include <instruction.hpp>

void instrumentMemoryAccess(byte *opcodes, triton::arch::Instruction* tritonInst);