#pragma once

void set_automatic_tainting();
void get_tainted_operands_and_add_comment(triton::arch::Instruction* tritonInst, ea_t pc);