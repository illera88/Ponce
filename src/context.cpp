//! \file
/*
**  Copyright (c) 2020 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

#include <cstring>
#include <stdexcept>

//Triton
#include <triton/cpuSize.hpp>
#include <triton/coreUtils.hpp>
#include <triton/x86Specifications.hpp>
#include <triton/context.hpp>

#include "context.hpp"
#include "globals.hpp"

//IDA
#include <dbg.hpp>
#include <pro.h>
#include <bytes.hpp>
//Ponce
#include "globals.hpp"

/* Get a memory value from IDA debugger*/
triton::uint512 IDA_getCurrentMemoryValue(ea_t addr, triton::uint32 size)
{
    if (size > triton::size::max_supported) {
        warning("[!] getCurrentMemoryValue() error, size can't be larger than %u bytes (%u)\n", triton::size::max_supported, triton::size::max_supported * 8);
        return -1;
    }
    triton::uint8 buffer[64] = { 0 };
    //This is the way to force IDA to read the value from the debugger
    //More info here: https://www.hex-rays.com/products/ida/support/sdkdoc/dbg_8hpp.html#ac67a564945a2c1721691aa2f657a908c
    invalidate_dbgmem_contents(addr, size);
    get_bytes(&buffer, size, addr, GMB_READALL, NULL);

    triton::uint512 value = 0;
    switch (size) {
    case triton::size::byte:    value = *(reinterpret_cast<triton::uint8*>(buffer));  break;
    case triton::size::word:    value = *(reinterpret_cast<triton::uint16*>(buffer)); break;
    case triton::size::dword:   value = *(reinterpret_cast<triton::uint32*>(buffer)); break;
    case triton::size::qword:   value = *(reinterpret_cast<triton::uint64*>(buffer)); break;
    case triton::size::dqword:  value = triton::utils::cast<triton::uint128>(reinterpret_cast<triton::uint8*>(buffer)); break;
    case triton::size::qqword:  value = triton::utils::cast<triton::uint256>(reinterpret_cast<triton::uint8*>(buffer)); break;
    case triton::size::dqqword: value = triton::utils::cast<triton::uint512>(reinterpret_cast<triton::uint8*>(buffer)); break;
    }

    return value;
    return triton::utils::cast<triton::uint512>(buffer);
}

/*This callback is called when triton is processing a instruction and it needs a memory value to build the expressions*/
void needConcreteMemoryValue_cb(triton::Context& ctx, const triton::arch::MemoryAccess& mem)
{
    bool had_it = false;
    auto IDA_memValue = IDA_getCurrentMemoryValue((ea_t)mem.getAddress(), mem.getSize());

    if (tritonCtx.isConcreteMemoryValueDefined(mem)) {
        auto triton_memValue = tritonCtx.getConcreteMemoryValue(mem, false);
        if ((IDA_memValue != triton_memValue)) {
            tritonCtx.setConcreteMemoryValue(mem, IDA_memValue);
            had_it = false;
        }
        had_it = true;
    }
    else {
        tritonCtx.setConcreteMemoryValue(mem, IDA_memValue);
    }

    if (cmdOptions.showExtraDebugInfo) {
        char ascii_value[5] = { 0 };
        if (std::isprint(static_cast<unsigned char>(IDA_memValue)))
            qsnprintf(ascii_value, sizeof(ascii_value), "(%c)", static_cast<char>(IDA_memValue));
        std::stringstream stream;
        stream << std::nouppercase << std::hex << IDA_memValue;
        msg("[+] Triton asking IDA for %s syncronized memory address: " MEM_FORMAT " Size: %u. Value: 0x%s %s\n", 
            had_it ? "already" : "not", 
            (ea_t)mem.getAddress(), 
            mem.getSize(), 
            stream.str().c_str(),
            std::isprint(static_cast<unsigned char>(IDA_memValue)) ? ascii_value : "");
    }
}

/* Get a reg value from IDA debugger*/
triton::uint512 IDA_getCurrentRegisterValue(const triton::arch::Register& reg)
{
    regval_t reg_value;
    triton::uint512 value = 0;
    auto reg_name = reg.getName();
    assert(!reg_name.empty());
    //We need to invalidate the registers. If not IDA uses the last value when program was stopped
    invalidate_dbg_state(DBGINV_REGS);
    get_reg_val(reg_name.c_str(), &reg_value);
    value = reg_value.ival;
    /* Sync with the libTriton */
    triton::arch::Register syncReg;
    if (reg.getId() >= tritonCtx.registers.x86_af.getId() &&
        reg.getId() <= tritonCtx.registers.x86_zf.getId()) {
        syncReg = tritonCtx.registers.x86_eflags;
    }  
    else if (reg.getId() >= tritonCtx.registers.x86_sse_ie.getId() &&
               reg.getId() <= tritonCtx.registers.x86_sse_fz.getId()) {
        syncReg = tritonCtx.registers.x86_mxcsr;
    }    
    else {
        syncReg = tritonCtx.getRegister(reg.getParent());
    } 

    return value;
}

/*This callback is called when triton is processing a instruction and it needs a regiter to build the expressions*/
void needConcreteRegisterValue_cb(triton::Context& ctx, const triton::arch::Register& reg)
{
    bool had_it = true;
    auto IDA_regValue = IDA_getCurrentRegisterValue(reg);
    auto triton_regValue = tritonCtx.getConcreteRegisterValue(reg, false);

    if (IDA_regValue != triton_regValue) {
        tritonCtx.setConcreteRegisterValue(reg, IDA_regValue);
        had_it = false;
    }
 
    if (cmdOptions.showExtraDebugInfo) {
        char ascii_value[5] = { 0 };
        if (std::isprint(static_cast<unsigned char>(IDA_regValue)))
            qsnprintf(ascii_value, sizeof(ascii_value), "(%c)", static_cast<char>(IDA_regValue));
        
        std::stringstream stream;
        stream << std::nouppercase << std::hex << IDA_regValue;
        msg("[+] Triton asking IDA for %s syncronized register: %s. IDA returns value: 0x%s %s\n",
            had_it ? "already" : "not",
            reg.getName().c_str(),
            stream.str().c_str(),
            std::isprint(static_cast<unsigned char>(IDA_regValue)) ? ascii_value : "");
    }
}




