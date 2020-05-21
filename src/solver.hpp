#pragma once

#include <vector>

#include <triton/api.hpp>

#include <ida.hpp>

class Input
{
public:
    int path_constraint_index;

    // Memory or register operands involved on the Input
    std::vector <triton::arch::MemoryAccess> memOperand;
    std::vector <triton::arch::Register> regOperand;

    triton::uint64 srcAddr, dstAddr;

    //! Constructor.
    Input() : path_constraint_index(0), srcAddr(0), dstAddr(0)
    {
    }

    //! Destructor.
    ~Input()
    {
    }
};

void set_SMT_solution(const Input& input_ptr);
void negate_flag_condition(triton::arch::Instruction* triton_instruction);
std::vector<Input> solve_formula(ea_t pc, int path_constraint_index);
