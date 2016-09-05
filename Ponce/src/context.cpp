#include <cstring>
#include <stdexcept>

//Triton
#include <cpuSize.hpp>
#include <coreUtils.hpp>
#include <x86Specifications.hpp>
#include <api.hpp>
#include "context.hpp"
#include "globals.hpp"

//IDA
#include <dbg.hpp>
#include <pro.h>

//Ponce
#include "globals.hpp"

/*This callback is called when triton is processing a instruction and it needs a memory value to build the expressions*/
void __stdcall needConcreteMemoryValue(triton::arch::MemoryAccess& mem)
{
	if (EXTRADEBUG)
		msg("[+] We need memory! Address: "HEX_FORMAT" Size: %d\n", (unsigned int)mem.getAddress(), mem.getSize());
	auto memValue = getCurrentMemoryValue((triton::__uint)mem.getAddress(), mem.getSize());
	mem.setConcreteValue(memValue);
	triton::api.setConcreteMemoryValue(mem);
}

/*This callback is called when triton is processing a instruction and it needs a regiter to build the expressions*/
void __stdcall needConcreteRegisterValue(triton::arch::Register& reg)
{
	auto regValue = getCurrentRegisterValue(reg);
	if (EXTRADEBUG)
		msg("[+] We need a register! Register: %s Value: "HEX_FORMAT"\n", reg.getName().c_str(), (unsigned int)regValue);
	reg.setConcreteValue(regValue);
	triton::api.setConcreteRegisterValue(reg);
}

triton::uint512 getCurrentRegisterValue(triton::arch::Register& reg)
{
	regval_t reg_value;
	triton::uint512 value;
	//We need to invalidate the registers. If not IDA uses the last value when program was stopped
	invalidate_dbg_state(DBGINV_REGS);
	get_reg_val(reg.getName().c_str(), &reg_value);
	value = reg_value.ival; //TODO : reg_value->ival is ui64 won't work for xmm and larger registers

	/* Sync with the libTriton */
	triton::arch::Register syncReg;
	if (reg.getId() >= triton::arch::x86::ID_REG_AF && reg.getId() <= triton::arch::x86::ID_REG_ZF)
		syncReg = TRITON_X86_REG_EFLAGS;
	else if (reg.getId() >= triton::arch::x86::ID_REG_IE && reg.getId() <= triton::arch::x86::ID_REG_FZ)
		syncReg = TRITON_X86_REG_MXCSR;
	else
		syncReg = reg.getParent();

	//value = triton::utils::fromBufferToUint<triton::uint512>(buffer);
	syncReg.setConcreteValue(value);
	triton::api.setConcreteRegisterValue(syncReg);
	/* Returns the good casted value */
	return triton::api.getConcreteRegisterValue(reg, false);
}

triton::uint128 getCurrentMemoryValue(triton::__uint addr, triton::uint32 size) 
{
	if (size > 16){
		//msg("[!]Error, size can't be larger than 16\n"); 
	}
	triton::uint128 value = 0;
	//This is the way to force IDA to read the value from the debugger
	//More info here: https://www.hex-rays.com/products/ida/support/sdkdoc/dbg_8hpp.html#ac67a564945a2c1721691aa2f657a908c
	invalidate_dbgmem_contents(addr, sizeof(value));
	get_many_bytes(addr, &value, size);
	return value;
}