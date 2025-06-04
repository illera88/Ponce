//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the BSD License.
*/

#include <iostream>

#include "snapshot.hpp"
#include "globals.hpp"
#include "utils.hpp"

#include "dbg.hpp"

Snapshot::Snapshot() {
    this->locked = true;
    this->snapshotTaintEngine = nullptr;
    this->snapshotSymEngine = nullptr;
    this->astCtx = nullptr;
    this->cpu_x8664 = nullptr;
    this->cpu_x86 = nullptr;
    this->cpu_AArch64 = nullptr;
    this->cpu_Arm32 = nullptr;
    this->mustBeRestore = false;
    this->snapshotTaken = false;
}


Snapshot::~Snapshot() {
    resetEngine();
}

/* Check if the snapshot has been taken */
bool Snapshot::exists(void) {
    return this->snapshotTaken;
}

/* Add the modification byte. */
void Snapshot::addModification(ea_t mem, char byte) {
    if (this->locked == false && this->memory.find(mem) == this->memory.end())
        this->memory[mem] = byte;
}


/* Enable the snapshot engine. */
void Snapshot::takeSnapshot() {
    if (this->snapshotTaken) {
        resetEngine();
    }

    this->snapshotTaken = true;

    /* 1 - Unlock the engine */
    this->locked = false;

    /* 2 - Save current symbolic engine state */
    this->snapshotSymEngine = new triton::engines::symbolic::SymbolicEngine(*tritonCtx.getSymbolicEngine());

    /* 3 - Save current taint engine state */
    this->snapshotTaintEngine = new triton::engines::taint::TaintEngine(*tritonCtx.getTaintEngine());

    /* 4 - Save current set of nodes */
    this->astCtx = new triton::ast::AstContext(*tritonCtx.getAstContext());
    //this->nodesList = tritonCtx.getAllocatedAstNodes();

    /* 5 - Save the Triton CPU state. It depens on the analyzed binary*/   
    switch (tritonCtx.getArchitecture()) {
    case triton::arch::ARCH_X86_64:
        this->cpu_x8664 = new triton::arch::x86::x8664Cpu(*dynamic_cast<triton::arch::x86::x8664Cpu*>(tritonCtx.getCpuInstance()));
        break;
    case triton::arch::ARCH_X86:
        this->cpu_x86 = new triton::arch::x86::x86Cpu(*reinterpret_cast<triton::arch::x86::x86Cpu*>(tritonCtx.getCpuInstance()));
        break;
    case triton::arch::ARCH_AARCH64:
        this->cpu_AArch64 = new triton::arch::arm::aarch64::AArch64Cpu(*reinterpret_cast<triton::arch::arm::aarch64::AArch64Cpu*>(tritonCtx.getCpuInstance()));
        break;
    case triton::arch::ARCH_ARM32:
        this->cpu_Arm32 = new triton::arch::arm::arm32::Arm32Cpu(*reinterpret_cast<triton::arch::arm::arm32::Arm32Cpu*>(tritonCtx.getCpuInstance()));
        break;
    default:
        throw triton::exceptions::Architecture("Architecture not supported.");
        break;
    }

    /* 6 - Save IDA registers context */
    auto regs = tritonCtx.getAllRegisters();
    for (auto it = regs.begin(); it != regs.end(); it++)
    {
        const triton::arch::Register reg = (*it).second;
        uint64 ival;
        if (get_reg_val(reg.getName().c_str(), &ival)) {
            this->IDAContext[reg.getName()] = ival;
        }
    }

    //We also saved the ponce status
    this->saved_ponce_runtime_status = ponce_runtime_status;
}

void Snapshot::setAddress(ea_t address) {
    this->address = address;
}


/* Restore the snapshot. */
void Snapshot::restoreSnapshot() {

    /* 1 - Restore all memory modification. */
    for (auto i = this->memory.begin(); i != this->memory.end(); ++i) {
        put_bytes(i->first, &i->second, 1);
    }
    this->memory.clear();

    /* 2 - Restore current symbolic engine state */
    *tritonCtx.getSymbolicEngine() = *this->snapshotSymEngine;

    /* 3 - Restore current taint engine state */
    *tritonCtx.getTaintEngine() = *this->snapshotTaintEngine;

    /* 4 - Restore current AST context */
    *tritonCtx.getAstContext() = *this->astCtx;

    /* 5 - Restore the Triton CPU state */
    switch (tritonCtx.getArchitecture()) {
    case triton::arch::ARCH_X86_64:
        *reinterpret_cast<triton::arch::x86::x8664Cpu*>(tritonCtx.getCpuInstance()) = *this->cpu_x8664;
        break;
    case triton::arch::ARCH_X86:
        *reinterpret_cast<triton::arch::x86::x86Cpu*>(tritonCtx.getCpuInstance()) = *this->cpu_x86;
        break;
    case triton::arch::ARCH_AARCH64:
        *reinterpret_cast<triton::arch::arm::aarch64::AArch64Cpu*>(tritonCtx.getCpuInstance()) = *this->cpu_AArch64;
        break;
    case triton::arch::ARCH_ARM32:
        *reinterpret_cast<triton::arch::arm::arm32::Arm32Cpu*>(tritonCtx.getCpuInstance()) = *this->cpu_Arm32;
        break;
    default:
        throw triton::exceptions::Architecture("Architecture not supported.");
        break;
    }

    this->mustBeRestore = false;

    /* 6 - Restore IDA registers context
    Suposedly XIP should be set at the same time and execution redirected*/
    typedef std::map<std::string, triton::uint512>::iterator it_type;
    for (it_type iterator = this->IDAContext.begin(); iterator != this->IDAContext.end(); iterator++) {
        if (!set_reg_val(iterator->first.c_str(), static_cast<uint64>(iterator->second)))
            msg("[!] ERROR restoring register %s\n", iterator->first.c_str());
    }

    /* 7 - Restore the Ponce status */
    ponce_runtime_status = this->saved_ponce_runtime_status;

    /* 8 - We need to set to NULL the last instruction. We are deleting the last instructions in the Tritonize callback.
    So after restore a snapshot if last_instruction is not NULL is double freeing the same instruction */
    ponce_runtime_status.last_triton_instruction = nullptr;
}

/* Disable the snapshot engine. */
void Snapshot::disableSnapshot(void) {
    this->locked = true;
}


/* Reset the snapshot engine.
* Clear all backups for a new snapshot. */
void Snapshot::resetEngine(void) {
    if (!this->snapshotTaken)
        return;

    this->memory.clear();
    delete this->snapshotSymEngine;
    this->snapshotSymEngine = nullptr;

    delete this->snapshotTaintEngine;
    this->snapshotTaintEngine = nullptr;

    delete this->astCtx;
    this->astCtx = nullptr;

    // Fix Memleak
    switch (tritonCtx.getArchitecture()) {
    case triton::arch::ARCH_X86_64:
        delete this->cpu_x8664;
        this->cpu_x8664 = nullptr;
        break;
    case triton::arch::ARCH_X86:
        delete this->cpu_x86;
        this->cpu_x86 = nullptr;
        break;
    case triton::arch::ARCH_AARCH64:
        delete this->cpu_AArch64;
        this->cpu_AArch64 = nullptr;
        break;
    case triton::arch::ARCH_ARM32:
        delete this->cpu_Arm32;
        this->cpu_Arm32 = nullptr;
        break;
    default:
        break;
    }

    this->snapshotTaken = false;

    //We delete the comment and color that we created
    ponce_set_cmt(this->address, "", false, false, false);
    del_item_color(this->address);
    this->address = 0;
}


/* Check if the snapshot engine is locked. */
bool Snapshot::isLocked(void) {
    return this->locked;
}


/* Check if we must restore the snapshot */
bool Snapshot::mustBeRestored(void) {
    return this->mustBeRestore;
}


/* Check if we must restore the snapshot */
void Snapshot::setRestore(bool flag) {
    this->mustBeRestore = flag;
}