//! \file
/*
**  Copyright (c) 2016 - Ponce
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
#include <triton/api.hpp>

#include "context.hpp"
#include "globals.hpp"

//IDA
#include <dbg.hpp>
#include <pro.h>
#include <bytes.hpp>
//Ponce
#include "globals.hpp"

/*This callback is called when triton is processing a instruction and it needs a memory value to build the expressions*/
void needConcreteMemoryValue(triton::API& api, const triton::arch::MemoryAccess& mem)
{
	if (cmdOptions.showExtraDebugInfo) {
		if (_is_64())
			msg("[+] We need memory! Address: %#llx Size: %u\n", (ea_t)mem.getAddress(), mem.getSize());
		else
			msg("[+] We need memory! Address: %#x Size: %u\n", (ea_t)mem.getAddress(), mem.getSize());
	}
	auto memValue = getCurrentMemoryValue((ea_t)mem.getAddress(), mem.getSize());
	api.setConcreteMemoryValue(mem, memValue);
	/*mem.setConcreteValue(memValue);
	api.setConcreteMemoryValue(mem);*/
}

/*This callback is called when triton is processing a instruction and it needs a regiter to build the expressions*/
void needConcreteRegisterValue(triton::API& api, const triton::arch::Register& reg)
{
	auto regValue = getCurrentRegisterValue(reg);
	if (cmdOptions.showExtraDebugInfo) {
		if (_is_64())
			msg("[+] We need a register! Register: %s Value: %#llx\n", reg.getName().c_str(), regValue.convert_to<ea_t>());
		else
			msg("[+] We need a register! Register: %s Value: %#x\n", reg.getName().c_str(), regValue.convert_to<ea_t>());
	}
	api.setConcreteRegisterValue(reg, regValue);
	/*reg.setConcreteValue(regValue);
	api.setConcreteRegisterValue(reg);*/
}

triton::uint512 getCurrentRegisterValue(const triton::arch::Register& reg)
{
	regval_t reg_value;
	triton::uint512 value = 0;
	//We need to invalidate the registers. If not IDA uses the last value when program was stopped
	invalidate_dbg_state(DBGINV_REGS);
	get_reg_val(reg.getName().c_str(), &reg_value);
	value = reg_value.ival;
	/* Sync with the libTriton */
	triton::arch::Register syncReg;
	if (reg.getId() >= api.registers.x86_af.getId() && reg.getId() <= api.registers.x86_zf.getId())
		syncReg = api.registers.x86_eflags;
	else if (reg.getId() >= api.registers.x86_ie.getId() && reg.getId() <= api.registers.x86_fz.getId())
		syncReg = api.registers.x86_mxcsr;
	else
		syncReg = api.getRegister(reg.getParent());

	api.setConcreteRegisterValue(syncReg, value);
	/* Returns the good casted value */
	return api.getConcreteRegisterValue(reg, false);
}


triton::uint128 getCurrentMemoryValue(ea_t addr, triton::uint32 size) 
{
	if (size > 16)
	{
		warning("[!] Error, size can't be larger than 16\n");
		return -1;
	}
	triton::uint8 buffer[16] = {0};
	//This is the way to force IDA to read the value from the debugger
	//More info here: https://www.hex-rays.com/products/ida/support/sdkdoc/dbg_8hpp.html#ac67a564945a2c1721691aa2f657a908c
	invalidate_dbgmem_contents(addr, size);
#if IDA_SDK_VERSION >=700
	get_bytes(&buffer, size, addr,GMB_READALL, NULL);
#else
	get_many_bytes(addr, &buffer, size);
#endif
	return triton::utils::fromBufferToUint<triton::uint128>(reinterpret_cast<triton::uint8*>(buffer));
}