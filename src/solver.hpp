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


std::vector<Input> solve_formula(ea_t pc, size_t path_constraint_index);
void negate_inject_maybe_restore_solver(ea_t pc, int path_constraint_index, bool restore);
