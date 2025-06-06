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
    try {
       if (this->snapshotTaken) {
           resetEngine();
       }
    } catch(...) {
         // Destructor must not throw
         msg("[!] Snapshot: Exception in destructor.\n");
    }
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

    /* Unlock the engine */
    this->locked = false;
    // snapshotTaken should only be set to true at the very end if everything succeeds
    try {
        /* Save current symbolic engine state */
        // WARNING: According to Triton issue #385, this creates a shallow copy
        // Do NOT delete this object in resetEngine() to prevent double free
        auto* symEngine = tritonCtx.getSymbolicEngine();
        if (symEngine == nullptr) {
            throw std::runtime_error("SymbolicEngine is null");
        }
        this->snapshotSymEngine = new triton::engines::symbolic::SymbolicEngine(*symEngine);

        /* Save current taint engine state */
        // WARNING: This is also likely a shallow copy
        auto* taintEngine = tritonCtx.getTaintEngine();
        if (taintEngine == nullptr) {
            throw std::runtime_error("TaintEngine is null");
        }
        this->snapshotTaintEngine = new triton::engines::taint::TaintEngine(*taintEngine);

        /* Save current set of nodes */
        // WARNING: This is also likely a shallow copy
        auto astContext = tritonCtx.getAstContext();
        if (astContext == nullptr) {
            throw std::runtime_error("AstContext is null");
        }
        this->astCtx = new triton::ast::AstContext(*astContext);
    //this->nodesList = tritonCtx.getAllocatedAstNodes();

        /* Save the Triton CPU state. It depends on the analyzed binary*/
        switch (tritonCtx.getArchitecture()) {
        case triton::arch::ARCH_X86_64: {
            auto* cpu_ptr = dynamic_cast<triton::arch::x86::x8664Cpu*>(tritonCtx.getCpuInstance());
            if (cpu_ptr == nullptr) {
                throw std::runtime_error("Failed to cast CPU instance to x8664Cpu");
            }
            this->cpu_x8664 = new triton::arch::x86::x8664Cpu(*cpu_ptr);
            break;
        }
        case triton::arch::ARCH_X86: {
            auto* cpu_ptr = dynamic_cast<triton::arch::x86::x86Cpu*>(tritonCtx.getCpuInstance());
            if (cpu_ptr == nullptr) {
                throw std::runtime_error("Failed to cast CPU instance to x86Cpu");
            }
            this->cpu_x86 = new triton::arch::x86::x86Cpu(*cpu_ptr);
            break;
        }
        case triton::arch::ARCH_AARCH64: {
            auto* cpu_ptr = dynamic_cast<triton::arch::arm::aarch64::AArch64Cpu*>(tritonCtx.getCpuInstance());
            if (cpu_ptr == nullptr) {
                throw std::runtime_error("Failed to cast CPU instance to AArch64Cpu");
            }
            this->cpu_AArch64 = new triton::arch::arm::aarch64::AArch64Cpu(*cpu_ptr);
            break;
        }
        case triton::arch::ARCH_ARM32: {
            auto* cpu_ptr = dynamic_cast<triton::arch::arm::arm32::Arm32Cpu*>(tritonCtx.getCpuInstance());
            if (cpu_ptr == nullptr) {
                throw std::runtime_error("Failed to cast CPU instance to Arm32Cpu");
            }
            this->cpu_Arm32 = new triton::arch::arm::arm32::Arm32Cpu(*cpu_ptr);
            break;
        }
        default:
            throw triton::exceptions::Architecture("Architecture not supported.");
        }

    /* Save IDA registers context */
    auto regs = tritonCtx.getAllRegisters();
    for (auto it = regs.begin(); it != regs.end(); it++)
    {
        const triton::arch::Register reg = (*it).second;
        uint64 ival;
        if (get_reg_val(reg.getName().c_str(), &ival)) {
            this->IDAContext[reg.getName()] = ival;
        }
    }
        // We also saved the ponce status
        this->saved_ponce_runtime_status = ponce_runtime_status;

        // Only set snapshotTaken to true if everything succeeded
        this->snapshotTaken = true;

    } catch (const std::exception& e) {
        msg("[!] Error taking snapshot: %s\n", e.what());

        delete this->snapshotSymEngine;   this->snapshotSymEngine = nullptr;
        delete this->snapshotTaintEngine; this->snapshotTaintEngine = nullptr;
        delete this->astCtx;              this->astCtx = nullptr;
        delete this->cpu_x8664;           this->cpu_x8664 = nullptr;
        delete this->cpu_x86;             this->cpu_x86 = nullptr;
        delete this->cpu_AArch64;         this->cpu_AArch64 = nullptr;
        delete this->cpu_Arm32;           this->cpu_Arm32 = nullptr;

        // Reset state
        this->locked = true;
        this->snapshotTaken = false;

        // Re-throw the exception
        throw;
    }
}

void Snapshot::setAddress(ea_t address) {
    this->address = address;
}


/* Restore the snapshot. */
void Snapshot::restoreSnapshot() {
    // Check if snapshot was actually taken
    if (!this->snapshotTaken) {
        msg("[!] Cannot restore snapshot: no snapshot was taken\n");
        return;
    }

    /* Restore all memory modification. */
    for (auto i = this->memory.begin(); i != this->memory.end(); ++i) {
        put_bytes(i->first, &i->second, 1);
    }
    this->memory.clear();

    /* Restore current symbolic engine state */
    if (this->snapshotSymEngine != nullptr) {
        auto* symEngine = tritonCtx.getSymbolicEngine();
        if (symEngine != nullptr) {
            *symEngine = *this->snapshotSymEngine;
        } else {
            msg("[!] ERROR: Current SymbolicEngine is null\n");
        }
    }

    /* Restore current taint engine state */
    if (this->snapshotTaintEngine != nullptr) {
        auto* taintEngine = tritonCtx.getTaintEngine();
        if (taintEngine != nullptr) {
            *taintEngine = *this->snapshotTaintEngine;
        } else {
            msg("[!] ERROR: Current TaintEngine is null\n");
        }
    }

    /* Restore current AST context */
    if (this->astCtx != nullptr) {
        auto astContext = tritonCtx.getAstContext();
        if (astContext != nullptr) {
            *astContext = *this->astCtx;
        } else {
            msg("[!] ERROR: Current AstContext is null\n");
        }
    }

    /* Restore the Triton CPU state */
    switch (tritonCtx.getArchitecture()) {
    case triton::arch::ARCH_X86_64: {
        if (this->cpu_x8664 != nullptr) {
            auto* current_cpu = dynamic_cast<triton::arch::x86::x8664Cpu*>(tritonCtx.getCpuInstance());
            if (current_cpu != nullptr) {
                *current_cpu = *this->cpu_x8664;
            } else {
                msg("[!] ERROR: Failed to cast current CPU instance to x8664Cpu\n");
            }
        } else {
            msg("[!] ERROR: Snapshot CPU x8664 is null\n");
        }
        break;
    }
    case triton::arch::ARCH_X86: {
        if (this->cpu_x86 != nullptr) {
            auto* current_cpu = dynamic_cast<triton::arch::x86::x86Cpu*>(tritonCtx.getCpuInstance());
            if (current_cpu != nullptr) {
                *current_cpu = *this->cpu_x86;
            } else {
                msg("[!] ERROR: Failed to cast current CPU instance to x86Cpu\n");
            }
        } else {
            msg("[!] ERROR: Snapshot CPU x86 is null\n");
        }
        break;
    }
    case triton::arch::ARCH_AARCH64: {
        if (this->cpu_AArch64 != nullptr) {
            auto* current_cpu = dynamic_cast<triton::arch::arm::aarch64::AArch64Cpu*>(tritonCtx.getCpuInstance());
            if (current_cpu != nullptr) {
                *current_cpu = *this->cpu_AArch64;
            } else {
                msg("[!] ERROR: Failed to cast current CPU instance to AArch64Cpu\n");
            }
        } else {
            msg("[!] ERROR: Snapshot CPU AArch64 is null\n");
        }
        break;
    }
    case triton::arch::ARCH_ARM32: {
        if (this->cpu_Arm32 != nullptr) {
            auto* current_cpu = dynamic_cast<triton::arch::arm::arm32::Arm32Cpu*>(tritonCtx.getCpuInstance());
            if (current_cpu != nullptr) {
                *current_cpu = *this->cpu_Arm32;
            } else {
                msg("[!] ERROR: Failed to cast current CPU instance to Arm32Cpu\n");
            }
        } else {
            msg("[!] ERROR: Snapshot CPU Arm32 is null\n");
        }
        break;
    }
    default:
        throw triton::exceptions::Architecture("Architecture not supported.");
    }

    this->mustBeRestore = false;

    /* Restore IDA registers context
    Suposedly XIP should be set at the same time and execution redirected*/
    typedef std::map<std::string, triton::uint512>::iterator it_type;
    for (it_type iterator = this->IDAContext.begin(); iterator != this->IDAContext.end(); iterator++) {
        if (!set_reg_val(iterator->first.c_str(), static_cast<uint64>(iterator->second)))
            msg("[!] ERROR restoring register %s\n", iterator->first.c_str());
    }

    ponce_runtime_status = this->saved_ponce_runtime_status;

    /* We need to set to NULL the last instruction. We are deleting the last instructions in the Tritonize callback.
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

    this->snapshotSymEngine = nullptr;
    this->snapshotTaintEngine = nullptr;
    this->astCtx = nullptr;
    
    this->cpu_x8664 = nullptr; 
    this->cpu_x86 = nullptr;
    this->cpu_AArch64 = nullptr;
    // delete this->cpu_Arm32;   
     this->cpu_Arm32 = nullptr;

    // Clear IDA context and reset all state flags
    this->IDAContext.clear();
     this->snapshotTaken = false;
     this->locked = true;
     this->mustBeRestore = false;
   
    if (this->address != 0) {
       try {
         ponce_set_cmt(this->address, "", false, false, false);
         del_item_color(this->address);
       } catch (...) {
          // IDA API might fail during shutdown
           msg("[!] Snapshot::resetEngine: Error calling IDA API during cleanup.\n");
       }
    }
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