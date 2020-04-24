#pragma once

#include <vector>

#include <triton/api.hpp>

#include <ida.hpp>

class Input
{
public:
    int bound;
    std::vector <triton::arch::MemoryAccess> memOperand;
    std::vector <triton::arch::Register> regOperand;

    //! Constructor.
    Input()
    {
        this->bound = 0;
    }

    //! Destructor.
    ~Input()
    {
    }
};

void set_SMT_results(Input* input_ptr);
void negate_flag_condition(triton::arch::Instruction* triton_instruction);
Input* solve_formula(ea_t pc, uint bound);
