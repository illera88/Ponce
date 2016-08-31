#include <cstring>
#include <stdexcept>

/* Triton */
#include <cpuSize.hpp>
#include <coreUtils.hpp>
#include <x86Specifications.hpp>
#include <api.hpp>
#include "context.hpp"
#include "globals.hpp"

/*IDA*/
#include <dbg.hpp>
#include <pro.h>
//#include <kernwin.hpp>

//Ponce
#include "globals.hpp"

bool     mustBeExecuted = false;

triton::uint128 getCurrentMemoryValue(triton::__uint addr, triton::uint32 size);
triton::uint512 getCurrentRegisterValue(triton::arch::Register& reg);

//this next two callbacks are used to get memory and registers and update them.
void __cdecl needConcreteMemoryValue(triton::arch::MemoryAccess& mem) 
{
	if (EXTRADEBUG)
		msg("[+] We need memory! Address: "HEX_FORMAT" Size: %d\n", (unsigned int)mem.getAddress(), mem.getSize());
	auto memValue = getCurrentMemoryValue(mem.getAddress(), mem.getSize());
	mem.setConcreteValue(memValue);
	triton::api.setConcreteMemoryValue(mem);
}

void __cdecl needConcreteRegisterValue(triton::arch::Register& reg)
{
	if (EXTRADEBUG)
		msg("[+] We need a register! Register: %s\n", reg.getName().c_str());
	auto regValue = getCurrentRegisterValue(reg);
	reg.setConcreteValue(regValue);
	triton::api.setConcreteRegisterValue(reg);
}

triton::uint512 getCurrentRegisterValue(triton::arch::Register& reg)
{
	regval_t reg_value;
	triton::uint512 value;
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


triton::uint128 getCurrentMemoryValue(triton::arch::MemoryAccess& mem) {
	return getCurrentMemoryValue(mem.getAddress(), mem.getSize());
}

triton::uint128 getCurrentMemoryValue(triton::__uint addr) {
	//if (PIN_CheckReadAccess(reinterpret_cast<void*>(addr)) == false)
	//          throw std::runtime_error("tracer::pintool::context::getCurrentMemoryValue(): Page not readable.");
	
	return static_cast<triton::uint128>(*(reinterpret_cast<triton::uint8*>(addr)));
}


triton::uint128 getCurrentMemoryValue(triton::__uint addr, triton::uint32 size) {
	
	if (size > 16){
		//msg("[!]Error, size can't be larger than 16\n"); 
	}

	triton::uint128 value = 0;
	uint8_t *out = (uint8_t*)malloc(size + 1);
	get_many_bytes(addr, out, size);

	switch (size) {
	case BYTE_SIZE:   value = *(reinterpret_cast<triton::uint8*>(out));                            break;
	case WORD_SIZE:   value = *(reinterpret_cast<triton::uint16*>(out));                           break;
	case DWORD_SIZE:  value = *(reinterpret_cast<triton::uint32*>(out));                           break;
	case QWORD_SIZE:  value = *(reinterpret_cast<triton::uint64*>(out));                           break;
	//case DQWORD_SIZE: value = triton::utils::fromBufferToUint<triton::uint128>(reinterpret_cast<triton::uint8*>(out)); break;
	default: break;
		//msg("Error, the specified size (%u) cannot be retrieved\n", size); 
	}
	free(out);
	return value;
}


/*void setCurrentRegisterValue(triton::arch::Register& reg) {
	setCurrentRegisterValue(reg, reg.getConcreteValue());
}*/


//void setCurrentRegisterValue(triton::arch::Register& reg, triton::uint512 value) {
//
//	set_reg_val(reg.getName().c_str(), value.convert_to<uint64>());
//
//
//	if (reg.getId() != reg.getParent().getId() || reg.isFlag())
//		throw std::runtime_error("tracer::pintool::context::setCurrentRegisterValue(): You cannot set a Pin register value on a sub-register or a flag.");
//
//	/* Sync with the libTriton */
//	triton::arch::Register syncReg(reg);
//	syncReg.setConcreteValue(value);
//	triton::api.setConcreteRegisterValue(syncReg);
//
//	/* We must concretize the register because the last symbolic value is now false */
//	triton::api.concretizeRegister(reg);
//}
//
//void setCurrentMemoryValue(triton::arch::MemoryAccess& mem) {
//	setCurrentMemoryValue(mem, mem.getConcreteValue());
//}
//
//
//void setCurrentMemoryValue(triton::arch::MemoryAccess& mem, triton::uint512 value) {
//	triton::__uint  addr = mem.getAddress();
//	triton::uint32  size = mem.getSize();
//
//	/* Sync with the libTriton */
//	mem.setConcreteValue(value);
//	triton::api.setConcreteMemoryValue(mem);
//
//	/* We must concretize the memory because the last symbolic value is now false */
//	triton::api.concretizeMemory(mem);
//
//	/* Inject memory value */
//	for (triton::uint32 i = 0; i < size; i++) {
//		patch_byte((addr + i),value.convert_to<uint32>());
//		//*((triton::uint8 *)(addr + i)) = (value & 0xff).convert_to<triton::uint8>();
//		value >>= 8;
//	}
//}
//
//
//void executeContext(void) {
//	//if (tracer::pintool::context::mustBeExecuted == true) {
//	//  PIN_UnlockClient();
//	//  PIN_ExecuteAt(tracer::pintool::context::lastContext);
//	//}
//}


//void setupContextRegister(triton::arch::Instruction* inst, dr_mcontext_t* ctx) {
//
//
//
//#if defined(__x86_64__) || defined(_M_X64)
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_RAX, ctx->rax));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_RBX, ctx->rbx));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_RCX, ctx->rcx));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_RDX, ctx->rdx));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_RDI, ctx->rdi));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_RSI, ctx->rsi));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_RBP, ctx->rbp));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_RSP, ctx->rsp));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_RIP, (reg_t)ctx->rip));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_EFLAGS, ctx->rflags));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_R8, ctx->r8));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_R9, ctx->r9));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_R10, ctx->r10));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_R11, ctx->r11));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_R12, ctx->r12));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_R13, ctx->r13));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_R14, ctx->r14));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_R15, ctx->r15));
//	/*inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM0, ctx->ymm[0]));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM1, ctx->ymm[1]));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM2, ctx->ymm[2]));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM3, ctx->ymm[3]));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM4, ctx->ymm[4]));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM5, ctx->ymm[5]));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM6, ctx->ymm[6]));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM7, ctx->ymm[7]));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM8, ctx->ymm));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM9, triton::fromBufferToUint128(buffer)));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM10, triton::fromBufferToUint128(buffer)));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM11, triton::fromBufferToUint128(buffer)));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM12, triton::fromBufferToUint128(buffer)));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM13, triton::fromBufferToUint128(buffer)));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM14, triton::fromBufferToUint128(buffer)));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM15, triton::fromBufferToUint128(buffer)));*/
//#endif
//
//#if defined(__i386) || defined(_M_IX86)
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_EAX, ctx->eax));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_EBX, ctx->ebx));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_ECX, ctx->ecx));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_EDX, ctx->edx));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_EDI, ctx->edi));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_ESI, ctx->esi));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_EBP, ctx->ebp));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_ESP, ctx->esp));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_EIP, (reg_t)ctx->eip));
//	inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_EFLAGS, ctx->eflags));
//	//inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM0, triton::fromBufferToUint128(buffer)));
//	//inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM1, triton::fromBufferToUint128(buffer)));
//	//inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM2, triton::fromBufferToUint128(buffer)));
//	//inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM3, triton::fromBufferToUint128(buffer)));
//	//inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM4, triton::fromBufferToUint128(buffer)));
//	//inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM5, triton::fromBufferToUint128(buffer)));
//	//inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM6, triton::fromBufferToUint128(buffer)));
//	//inst->updateContext(triton::arch::Register(triton::arch::x86::ID_REG_XMM7, triton::fromBufferToUint128(buffer)));
//#endif
//}
