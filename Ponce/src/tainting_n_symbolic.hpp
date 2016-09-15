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

void set_automatic_taint_n_simbolic();
void get_controlled_operands_and_add_comment(triton::arch::Instruction* tritonInst, ea_t pc);
//std::list<triton::arch::OperandWrapper> get_tainted_regs_operands(triton::arch::Instruction* tritonInst);