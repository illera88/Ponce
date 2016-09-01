#pragma once

void set_automatic_tainting();
void get_tainted_operands_and_add_comment(triton::arch::Instruction* tritonInst, ea_t pc);
//std::list<triton::arch::OperandWrapper> get_tainted_regs_operands(triton::arch::Instruction* tritonInst);