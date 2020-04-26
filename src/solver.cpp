
#include "solver.hpp"
#include "globals.hpp"

#include <dbg.hpp>

/* This function return a vector of Inputs. A vector is necesary since switch conditions may have multiple branch constraints*/
std::vector<Input> solve_formula(ea_t pc, uint bound)
{
    auto ast = api.getAstContext();
    std::vector<Input> solutions;

    if (api.getPathConstraints().size() - 1 < bound) {
        msg("Error. Requested bound %u is larger than PathConstraints vector size (%u)\n", bound, api.getPathConstraints().size());
        return solutions;
    }

    const auto& path_cons = api.getPathConstraints()[bound];
    assert(std::get<1>(path_cons.getBranchConstraints()[0]) == pc);

    if (path_cons.getTakenPredicate()->isSymbolized() && path_cons.isMultipleBranches()) {
        for (auto const& [taken, srcAddr, dstAddr, constraint] : path_cons.getBranchConstraints()) {
            if (!taken) {
                auto final_expr = ast->land(std::vector({ ast->equal(ast->bv(1, 1), ast->bv(1, 1)), constraint }));
                if (cmdOptions.showExtraDebugInfo) {
                    std::stringstream ss;
                    /*Create the full formula*/
                    ss << "(set-logic QF_AUFBV)\n";
                    /* Then, delcare all symbolic variables */
                    for (auto it : api.getSymbolicVariables()) {
                        ss << ast->declare(ast->variable(it.second));

                    }
                    /* And concat the user expression */
                    ss << "\n\n";
                    ss << final_expr;
                    ss << "\n(check-sat)";
                    ss << "\n(get-model)";
                    msg("[+] Formula:\n%s\n\n", ss.str().c_str());
                }

                //Time to solve
                auto model = api.getModel(final_expr);

                if (model.size() > 0) {
                    Input newinput;
                    //Clone object 
                    newinput.bound = bound;
                    newinput.dstAddr = dstAddr;
                    newinput.srcAddr = srcAddr;

                    msg("[+] Solution found! Values:\n");
                    for (const auto& [symId, model] : model) {
                        triton::engines::symbolic::SharedSymbolicVariable  symbVar = api.getSymbolicVariable(symId);
                        std::string  symbVarComment = symbVar->getComment();
                        triton::uint512 model_value = model.getValue();
                        if (symbVar->getType() == triton::engines::symbolic::variable_e::MEMORY_VARIABLE) {
                            auto mem = triton::arch::MemoryAccess(symbVar->getOrigin(), symbVar->getSize() / 8);
                            newinput.memOperand.push_back(mem);
                            api.setConcreteMemoryValue(mem, model_value); // Why
                        }
                        else if (symbVar->getType() == triton::engines::symbolic::variable_e::REGISTER_VARIABLE) {
                            auto reg = triton::arch::Register(*api.getCpuInstance(), (triton::arch::register_e)symbVar->getOrigin());
                            newinput.regOperand.push_back(reg);
                            api.setConcreteRegisterValue(reg, model_value); // Why?
                            //ToDo: add concretizeRegister()??
                        }
                        //We represent the number different 
                        switch (symbVar->getSize())
                        {
                        case 8:
                            msg(" - %s (%s): %#02x (%c)\n", model.getVariable()->getName().c_str(), symbVarComment.c_str(), model_value.convert_to<uchar>(), model_value.convert_to<uchar>() == 0 ? ' ' : model_value.convert_to<uchar>());
                            break;
                        case 16:
                            msg(" - %s (%s): %#04x (%c%c)\n", model.getVariable()->getName().c_str(), symbVarComment.c_str(), model_value.convert_to<ushort>(), model_value.convert_to<uchar>() == 0 ? ' ' : model_value.convert_to<uchar>(), (unsigned char)(model_value.convert_to<ushort>() >> 8) == 0 ? ' ' : (unsigned char)(model_value.convert_to<ushort>() >> 8));
                            break;
                        case 32:
                            msg(" - %s (%s): %#08x\n", model.getVariable()->getName().c_str(), symbVarComment.c_str(), model_value.convert_to<uint32>());
                            break;
                        case 64:
                            msg(" - %s (%s): %#16llx\n", model.getVariable()->getName().c_str(), symbVarComment.c_str(), model_value.convert_to<uint64>());
                            break;
                        default:
                            msg("[!] Unsupported size for the symbolic variable: %s (%s)\n", model.getVariable()->getName().c_str(), symbVarComment.c_str()); // what about 128 - 512 registers? 
                        }
                    }
                    solutions.push_back(newinput);
                }
                else {
                    msg("[!] No solution found :(\n");
                }
            }
        }
    }
    return solutions;
}



/*This function identify the type of condition jmp and negate the flags to negate the jmp.
Probably it is possible to do this with the solver, adding more variable to the formula to
identify the flag of the conditions and get the values. But for now we are doing it in this way.*/
void negate_flag_condition(triton::arch::Instruction* triton_instruction)
{
    switch (triton_instruction->getType())
    {
    case triton::arch::x86::ID_INS_JA:
    {
        uint64 cf;
        get_reg_val("CF", &cf);
        uint64 zf;
        get_reg_val("ZF", &zf);
        if (cf == 0 && zf == 0)
        {
            cf = 1;
            zf = 1;
        }
        else
        {
            cf = 0;
            zf = 0;
        }
        set_reg_val("ZF", zf);
        set_reg_val("CF", cf);
        break;
    }
    case triton::arch::x86::ID_INS_JAE:
    {
        uint64 cf;
        get_reg_val("CF", &cf);
        uint64 zf;
        get_reg_val("ZF", &zf);
        if (cf == 0 || zf == 0)
        {
            cf = 1;
            zf = 1;
        }
        else
        {
            cf = 0;
            zf = 0;
        }
        set_reg_val("ZF", zf);
        set_reg_val("CF", cf);
        break;
    }
    case triton::arch::x86::ID_INS_JB:
    {
        uint64 cf;
        get_reg_val("CF", &cf);
        cf = !cf;
        set_reg_val("CF", cf);
        break;
    }
    case triton::arch::x86::ID_INS_JBE:
    {
        uint64 cf;
        get_reg_val("CF", &cf);
        uint64 zf;
        get_reg_val("ZF", &zf);
        if (cf == 1 || zf == 1)
        {
            cf = 0;
            zf = 0;
        }
        else
        {
            cf = 1;
            zf = 1;
        }
        set_reg_val("ZF", zf);
        set_reg_val("CF", cf);
        break;
    }
    /*	ToDo: Check this one
        case triton::arch::x86::ID_INS_JCXZ:
        {
        break;
        }*/
    case triton::arch::x86::ID_INS_JE:
    case triton::arch::x86::ID_INS_JNE:
    {
        uint64 zf;
        auto old_value = get_reg_val("ZF", &zf);
        zf = !zf;
        set_reg_val("ZF", zf);
        break;
    }
    //case triton::arch::x86::ID_INS_JRCXZ:
    //case triton::arch::x86::ID_INS_JECXZ:
    case triton::arch::x86::ID_INS_JG:
    {
        uint64 sf;
        get_reg_val("SF", &sf);
        uint64 of;
        get_reg_val("OF", &of);
        uint64 zf;
        get_reg_val("ZF", &zf);
        if (sf == of && zf == 0)
        {
            sf = !of;
            zf = 1;
        }
        else
        {
            sf = of;
            zf = 0;
        }
        set_reg_val("SF", sf);
        set_reg_val("OF", of);
        set_reg_val("ZF", zf);
        break;
    }
    case triton::arch::x86::ID_INS_JGE:
    {
        uint64 sf;
        get_reg_val("SF", &sf);
        uint64 of;
        get_reg_val("OF", &of);
        uint64 zf;
        get_reg_val("ZF", &zf);
        if (sf == of || zf == 1)
        {
            sf = !of;
            zf = 0;
        }
        else
        {
            sf = of;
            zf = 1;
        }
        set_reg_val("SF", sf);
        set_reg_val("OF", of);
        set_reg_val("ZF", zf);
        break;
    }
    case triton::arch::x86::ID_INS_JL:
    {
        uint64 sf;
        get_reg_val("SF", &sf);
        uint64 of;
        get_reg_val("OF", &of);
        if (sf == of)
        {
            sf = !of;
        }
        else
        {
            sf = of;
        }
        set_reg_val("SF", sf);
        set_reg_val("OF", of);
        break;
    }
    case triton::arch::x86::ID_INS_JLE:
    {
        uint64 sf;
        get_reg_val("SF", &sf);
        uint64 of;
        get_reg_val("OF", &of);
        uint64 zf;
        get_reg_val("ZF", &zf);
        if (sf != of || zf == 1)
        {
            sf = of;
            zf = 0;
        }
        else
        {
            sf = !of;
            zf = 1;
        }
        set_reg_val("SF", sf);
        set_reg_val("OF", of);
        set_reg_val("ZF", zf);
        break;
    }
    case triton::arch::x86::ID_INS_JNO:
    case triton::arch::x86::ID_INS_JO:
    {
        uint64 of;
        get_reg_val("OF", &of);
        of = !of;
        set_reg_val("OF", of);
        break;
    }
    case triton::arch::x86::ID_INS_JNP:
    case triton::arch::x86::ID_INS_JP:
    {
        uint64 pf;
        get_reg_val("PF", &pf);
        pf = !pf;
        set_reg_val("PF", pf);
        break;
    }
    case triton::arch::x86::ID_INS_JNS:
    case triton::arch::x86::ID_INS_JS:
    {
        uint64 sf;
        get_reg_val("SF", &sf);
        sf = !sf;
        set_reg_val("SF", sf);
        break;
    }
    }
}


/*We set the memory to the results we got and do the analysis from there*/
void set_SMT_solution(Input input_ptr) {
    /*To set the memory types*/
    for (auto it = input_ptr.memOperand.begin(); it != input_ptr.memOperand.end(); it++)
    {
        auto concreteValue = api.getConcreteMemoryValue(*it, false);
        //auto concreteValue=it->getConcreteValue();
        put_bytes((ea_t)it->getAddress(), &concreteValue, it->getSize());

        api.setConcreteMemoryValue(*it, concreteValue);
        //We concretize the memory we set
        api.concretizeMemory(*it);
    }

    /*To set the register types*/
    for (auto it = input_ptr.regOperand.begin(); it != input_ptr.regOperand.end(); it++)
    {
        auto concreteRegValue = api.getConcreteRegisterValue(*it, false);
        set_reg_val(it->getName().c_str(), concreteRegValue.convert_to<uint64>());
        api.setConcreteRegisterValue(*it, concreteRegValue);
        //We concretize the register we set
        api.concretizeRegister(*it);
    }

    if (cmdOptions.showDebugInfo)
        msg("[+] Memory/Registers set with the SMT results\n");
}