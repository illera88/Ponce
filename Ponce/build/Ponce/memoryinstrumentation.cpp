


//IDA
#include <ida.hpp>

//Triton
//#include<api.h>
#include <memoryOperand.hpp>
//DR
#include <dr_api.h>

//Ponce
#include "globals.hpp"
#include "memoryinstrumentation.hpp"

void instrumentMemoryAccess(byte *opcodes, triton::arch::Instruction* tritonInst)
{
	/*Lets create a DynamoRio instr_t  object*/
	instr_t *instr=NULL;
	decode(GLOBAL_DCONTEXT, opcodes, instr);

	if (!instr_reads_memory(instr) && !instr_writes_memory(instr))
		return;
	/* insert code to add an entry for each memory reference opnd */
	for (int i = 0; i < instr_num_srcs(instr); i++)
	{
		if (opnd_is_memory_reference(instr_get_src(instr, i))){
			/*Add memory access <addr, size, content> */
			triton::arch::MemoryOperand read(11111111, 8, 401234);
		}
		//	instrument_mem(drcontext, bb, instr, instr_get_src(instr, i), false);
	}

	for (int i = 0; i < instr_num_dsts(instr); i++)
	{
		if (opnd_is_memory_reference(instr_get_dst(instr, i))){
			triton::arch::MemoryOperand write(11111111, 8, 401234);
		}
		//	instrument_mem(drcontext, bb, instr, instr_get_dst(instr, i), true);
	}

	//dr_insert_clean_call(drcontext, bb, instr, (void *)memtrace, false, 2,
	//	OPND_CREATE_INTPTR(tritonInst),
	//	OPND_CREATE_INTPTR(callback));
}