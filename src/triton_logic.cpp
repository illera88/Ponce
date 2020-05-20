
#include "triton_logic.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "context.hpp"
#include "blacklist.hpp"

#include <ida.hpp>
#include <dbg.hpp>
#include <auto.hpp>

/*This function will create and fill the Triton object for every instruction
    Returns:
    0 instruction tritonized
    1 trigger is not activated
    2 other error*/
int tritonize(ea_t pc, thid_t threadID)
{
    /*Check that the runtime Trigger is on just in case*/
    if (!ponce_runtime_status.runtimeTrigger.getState())
        return 1;

    threadID = threadID ? threadID : get_current_thread();

    if (pc == 0) {
        msg("[!] Some error at tritonize since pc is 0\n");
        return 2;
    }

    // Show analized instruction in IDA UI
    show_addr(pc);

    //We delete the last_instruction
    if (ponce_runtime_status.last_triton_instruction != nullptr) {
        delete ponce_runtime_status.last_triton_instruction;
        ponce_runtime_status.last_triton_instruction = nullptr;
    }

    triton::arch::Instruction* tritonInst = new triton::arch::Instruction();
    ponce_runtime_status.last_triton_instruction = tritonInst;

    /*This will fill the 'cmd' (to get the instruction size) which is a insn_t structure https://www.hex-rays.com/products/ida/support/sdkdoc/classinsn__t.html */
    if (!can_decode(pc)) {
        msg("[!] Some error decoding instruction at " MEM_FORMAT "\n", pc);
    }

    unsigned char opcodes[15];
    ssize_t item_size = 0x0;

    insn_t ins;
    decode_insn(&ins, pc);
    item_size = ins.size;
    assert(item_size < sizeof(opcodes));
    get_bytes(&opcodes, item_size, pc, GMB_READALL, NULL);

    /* Setup Triton information */
    tritonInst->clear(); // ToDo: I think this is not necesary
    tritonInst->setOpcode((triton::uint8*)opcodes, item_size);
    tritonInst->setAddress(pc);
    tritonInst->setThreadId(threadID);

    try {
        if (!api.processing(*tritonInst)) {
            msg("[!] Instruction at " MEM_FORMAT " not supported by Triton: %s (Thread id: %d)\n", pc, tritonInst->getDisassembly().c_str(), threadID);
            return 2;
        }
    }
    catch (const triton::exceptions::Exception& e) {
        msg("[!] Instruction at " MEM_FORMAT " not supported by Triton: %s (Thread id: %d)\n", pc, tritonInst->getDisassembly().c_str(), threadID);
        return 2;
    }

    if (cmdOptions.showExtraDebugInfo) {
        msg("[+] Triton at " MEM_FORMAT " : %s (Thread id: %d)\n", pc, tritonInst->getDisassembly().c_str(), threadID);
    }

    /*In the case that the snapshot engine is in use we should track every memory write access*/
    if (snapshot.exists())  {
        for (const auto& [memory_access, node]: tritonInst->getStoreAccess()){
            auto addr = memory_access.getAddress();
            //This is the way to force IDA to read the value from the debugger
            //More info here: https://www.hex-rays.com/products/ida/support/sdkdoc/dbg_8hpp.html#ac67a564945a2c1721691aa2f657a908c
            invalidate_dbgmem_contents((ea_t)addr, memory_access.getSize()); //ToDo: Do I have to call this for every byte in memory I want to read?
            for (unsigned int i = 0; i < memory_access.getSize(); i++) {
                triton::uint128 value = 0;
                //We get the memory readed
                get_bytes(&value, 1, (ea_t)addr + i, GMB_READALL, NULL);

                //We add a meomory modification to the snapshot engine
                snapshot.addModification((ea_t)addr + i, value.convert_to<char>());
            }
        }
    }

    /* Don't write nothing on symbolic/tainted branch instructions instructions because I'll do it later*/
    if (cmdOptions.addCommentsControlledOperands && !tritonInst->isBranch()){
        comment_controlled_operands(tritonInst, pc);
    }

    if (cmdOptions.addCommentsSymbolicExpresions)
        add_symbolic_expressions(tritonInst, pc);

    //We only paint the executed instructions if they don't have a previous color
    if (get_item_color(pc) == DEFCOLOR && cmdOptions.color_executed_instruction != DEFCOLOR) {
        ponce_set_item_color(pc, cmdOptions.color_executed_instruction);
    }
    

    //ToDo: The isSymbolized is missidentifying like "user-controlled" some instructions: https://github.com/JonathanSalwan/Triton/issues/383
    if (tritonInst->isTainted() || tritonInst->isSymbolized()) {
        ponce_runtime_status.total_number_symbolic_ins++;

        if (cmdOptions.showDebugInfo) {
            msg("[!] Instruction %s at " MEM_FORMAT " \n", tritonInst->isTainted() ? "tainted" : "symbolized", pc);
        }
        if (cmdOptions.RenameTaintedFunctionNames)
            rename_tainted_function(pc);
        // Check if it is a conditional jump
        // We only color with a different color the symbolic conditions, to show the user he could do additional actions like solve
        if (tritonInst->isBranch()) {
            if (tritonInst->isTainted())
                ponce_set_cmt(pc, "Tainted branch!", false, false);
            else
                ponce_set_cmt(pc, "Symbolic branch, make your choice!", false, false);

            ponce_runtime_status.total_number_symbolic_conditions++;
            if (cmdOptions.color_tainted_condition != DEFCOLOR)
                ponce_set_item_color(pc, cmdOptions.color_tainted_condition);
        }
        else  {
            //It paints every tainted/symbolic instruction
            if (cmdOptions.color_tainted != DEFCOLOR)
                ponce_set_item_color(pc, cmdOptions.color_tainted);
        }
    }

    if (tritonInst->isBranch() && tritonInst->isSymbolized()) {
        ea_t addr1 = (ea_t)tritonInst->getNextAddress();
        ea_t addr2 = (ea_t)tritonInst->operands[0].getImmediate().getValue();
        if (cmdOptions.showDebugInfo) {
            msg("[+] Branch symbolized detected at " MEM_FORMAT ": " MEM_FORMAT " or " MEM_FORMAT ", Taken:%s\n", pc, addr1, addr2, tritonInst->isConditionTaken() ? "Yes" : "No");
        }
    }
    return 0;
}

bool ponce_set_triton_architecture() {
    if (ph.id == PLFM_386) {
        if (ph.use64())
            api.setArchitecture(triton::arch::ARCH_X86_64);
        else if (ph.use32())
            api.setArchitecture(triton::arch::ARCH_X86);
        else {
            msg("[e] Wrong architecture\n");
            return false;
        }
    }
    else if (ph.id == PLFM_ARM) {
        if (ph.use64())
            api.setArchitecture(triton::arch::ARCH_AARCH64);
        else if (ph.use32())
            api.setArchitecture(triton::arch::ARCH_ARM32);
        else {
            msg("[e] Wrong architecture\n");
            return false;
        }
    }
    else {
        msg("[e] Architecture not supported by Ponce\n");
        return false;
    }
    return true;
}

/*This functions is called every time a new debugger session starts*/
void triton_restart_engines()
{
    if (cmdOptions.showDebugInfo)
        msg("[+] Restarting triton engines...\n");
    //We need to set the architecture for Triton
    ponce_set_triton_architecture();
    //We reset everything at the beginning
    api.reset();
    // Memory access callback
    api.addCallback(needConcreteMemoryValue_cb);
    // Register access callback
    api.addCallback(needConcreteRegisterValue_cb);
    //If we are in taint analysis mode we enable only the tainting engine and disable the symbolic one
    api.getTaintEngine()->enable(cmdOptions.use_tainting_engine);
    api.getSymbolicEngine()->enable(true);
    if (ponce_runtime_status.last_triton_instruction) {
        delete ponce_runtime_status.last_triton_instruction;
        ponce_runtime_status.last_triton_instruction = nullptr;
    }

    // This modes cannot be configured by the user. They are allways on
    api.setMode(triton::modes::ALIGNED_MEMORY, true);
    api.setMode(triton::modes::ONLY_ON_SYMBOLIZED, cmdOptions.use_symbolic_engine);
    api.setMode(triton::modes::ONLY_ON_TAINTED, cmdOptions.use_tainting_engine);
    // We don't want to track non symbolic path constraints
    api.setMode(triton::modes::PC_TRACKING_SYMBOLIC, true);

    // Set the optimizations selected by user
    api.setMode(triton::modes::AST_OPTIMIZATIONS, cmdOptions.AST_OPTIMIZATIONS);
    api.setMode(triton::modes::CONCRETIZE_UNDEFINED_REGISTERS, cmdOptions.CONCRETIZE_UNDEFINED_REGISTERS);
    api.setMode(triton::modes::CONSTANT_FOLDING, cmdOptions.CONSTANT_FOLDING);
    api.setMode(triton::modes::SYMBOLIZE_INDEX_ROTATION, cmdOptions.SYMBOLIZE_INDEX_ROTATION);
    api.setMode(triton::modes::TAINT_THROUGH_POINTERS, cmdOptions.TAINT_THROUGH_POINTERS);

    ponce_runtime_status.runtimeTrigger.disable();
    ponce_runtime_status.tainted_functions_index = 0;
    //Reset instruction counter
    ponce_runtime_status.total_number_traced_ins = 0;
    ponce_runtime_status.total_number_symbolic_ins = 0;
    ponce_runtime_status.total_number_symbolic_conditions = 0;
    ponce_runtime_status.current_trace_counter = 0;
    breakpoint_pending_actions.clear();
    if(ponce_table_chooser) ponce_table_chooser->constrains.clear();
    clear_requests_queue();

}


/*This function is call the first time we are tainting something to enable the trigger, the flags and the tracing*/
void start_tainting_or_symbolic_analysis()
{
    if (!ponce_runtime_status.runtimeTrigger.getState())
    {
        triton_restart_engines();
        // Delete previous Ponce comments
        delete_ponce_comments();
        ponce_runtime_status.runtimeTrigger.enable();
        ponce_runtime_status.analyzed_thread = get_current_thread();
        enable_step_trace(true);
        set_step_trace_options(0);
        ponce_runtime_status.tracing_start_time = 0;
    }

}