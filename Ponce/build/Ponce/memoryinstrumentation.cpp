


////IDA
#include <ida.hpp>
#include <intel.hpp>
//
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

insn_t at(ea_t ea){

	int length = decode_insn(ea); //to create cmd
	return cmd;
}

uint32 feature(ea_t ea){
	return cmd.get_canon_feature();
}


/*Return the number of operands of given instruction*/
int ops_count(ea_t ea){
	insn_t ins = at(ea);
	for (int i = 0; i < UA_MAXOP; i++)
		if (ins.Operands[i].type == o_void)
			return i;
}


for i in range(ops_count(0x0030BC6E)) :
 print f&read[i], f&write[i]


/*
def ops_state(ea):
	
    read = [ getattr(idaapi, 'CF_USE%d'%n) for n in range(1,7) ]
    write = [ getattr(idaapi, 'CF_CHG%d'%n) for n in range(1,7) ]
    f = feature(ea)
    res = ( ((f&read[i]),(f&write[i])) for i in range(ops_count(ea)) )
	return [ (r and 'r' or '') + (w and 'w' or '') for r,w in res ]*/

/*Returns 'r','w','rw' for each operand of an instruction*/
void ops_state(ea_t ea){
	const int read[] = { CF_USE1, CF_USE2, CF_USE3, CF_USE4, CF_USE5, CF_USE6};
	const int write[] = { CF_CHG1, CF_CHG2, CF_CHG3, CF_CHG4, CF_CHG5, CF_CHG6};
	uint32 f = feature(ea);
	for (int i = 0; i < ops_count(ea); i++){

	}
}

bool ops_read(ea_t ea){
	const int read[] = { CF_USE1, CF_USE2, CF_USE3, CF_USE4, CF_USE5, CF_USE6 };
	uint32 f = feature(ea);
	bool read = false;
	for (int i = 0; i < ops_count(ea); i++){

	}
}

bool ops_write(ea_t ea){
	const int write[] = { CF_CHG1, CF_CHG2, CF_CHG3, CF_CHG4, CF_CHG5, CF_CHG6 };
	uint32 f = feature(ea);
	bool write = false;
	for (int i = 0; i < ops_count(ea); i++){
		for (int = j; j < sizeof(write), j++){
			if (write[j] & f != 0)
				return true;
		}
	}
	return false;
}
for r,w in res:
 print r,w