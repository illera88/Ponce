//! \file
/*
**  Copyright (c) 2016 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

//Ponce
#include "callbacks.hpp"
#include "globals.hpp"
#include "utils.hpp"

//IDA
#include <ida.hpp>
#include <dbg.hpp>

//Triton
#include <api.hpp>
#include "x86Specifications.hpp"

void taint_or_symbolize_main_callback(ea_t main_address)
{
	//We need to invalidate the memory to forze IDA to reaload all the segments and new allocs
	invalidate_dbgmem_config();

	//Iterate through argc, argv[argc] and taint every byte and argc
	ea_t argc = get_args(0, true);
	ea_t argv = get_args(1, true);
	//msg("argc: %d argv: " HEX_FORMAT "\n", (unsigned int)argc, argv);
	if (cmdOptions.taintArgc)
	{
		//First we taint the argc
#if !defined(__EA64__)
		//In x86 we taint the memory of the first arg, argc
		//msg("[!] %s argc at memory: " HEX_FORMAT "\n", cmdOptions.use_tainting_engine? "Tainting": "Symbolizing", get_args_pointer(0, true));
		if (cmdOptions.use_tainting_engine)
			triton::api.taintMemory(triton::arch::MemoryAccess(get_args_pointer(0, true), 4, argc));
		else 
			triton::api.convertMemoryToSymbolicVariable(triton::arch::MemoryAccess(get_args_pointer(0, true), 4, argc), "argc");
		if (cmdOptions.showDebugInfo)
			msg("[!] argc %s\n", cmdOptions.use_tainting_engine ? "Tainted" : "Symbolized");
#else
		triton::arch::Register reg;
#ifdef __NT__ 
		reg = triton::arch::x86::x86_reg_rcx;
#elif __LINUX__ || __MAC__
		reg = triton::arch::x86::x86_reg_rdi;
#endif
		reg.setConcreteValue(argc);
		if (cmdOptions.use_tainting_engine)
			triton::api.taintRegister(reg);
		else
			triton::api.convertRegisterToSymbolicVariable(reg, "argc");

		if (cmdOptions.showDebugInfo)
			msg("[!] argc (%s) %s\n", reg.getName().c_str(), cmdOptions.use_tainting_engine ? "Tainted" : "Symbolized");
#endif	
		start_tainting_or_symbolic_analysis();
	}
	//Second we taint all the arguments values
	// We should first see if we are tainting main or wmain (Unicode)
	bool unicode = false;
	ea_t main_function = find_function("wmain");
	if (main_function != - 1)
		unicode = true;
	else
	{
		//Maybe we should look for more? _tmain?
		main_function = find_function("_wmain");
		if (main_function != -1)
			unicode = true;
		// If wmain or _wmain where not found it means that wmain was the function we found before
		// Unexpected behaviour if main and wmain exists
	}

	triton::uint32 char_size = unicode? 2: 1;// "char_size" in unicode is 2
	const void* null_byte =	unicode? "\0\0": "\0";
    uint32 reg_size = 4;
	if (inf.is_64bit())
        reg_size = 8;

	//We are tainting the argv[0], this is the program path, and it is something that the 
	//user controls and sometimes is used to do somechecks
	for (unsigned int i = cmdOptions.taintArgv0 ? 0 : 1; i < argc; i++)
	{
		ea_t current_argv = read_regSize_from_ida(argv + i * reg_size);
		if (current_argv == (ea_t)-1)
		{
			if (inf.is_64bit())
				msg("[!] Error reading mem: %#llx\n", argv + i * reg_size);
			else
				msg("[!] Error reading mem: %#x\n", argv + i * reg_size);
			break;
		}
		//We iterate through all the bytes of the current argument
		int j = 0;
		short current_char;
		do
		{
			if (unicode)
				current_char = read_unicode_char_from_ida(current_argv + j*char_size);
			else		
				current_char = read_char_from_ida(current_argv + j*char_size);

			if (memcmp(&current_char, null_byte, char_size) == 0 && !cmdOptions.taintEndOfString)
			{
				break;
			}
			
			if (cmdOptions.showExtraDebugInfo)
				msg("[!] %s argv[%d][%d]: %c\n", cmdOptions.use_tainting_engine ? "Tainting" : "Symbolizing", i, j, (char)current_char == 0 ? ' ' : (char)current_char);
			if (cmdOptions.use_tainting_engine)
				triton::api.taintMemory(triton::arch::MemoryAccess(current_argv + j*char_size, char_size, current_char));
			else
			{
				char comment[256];
				qsnprintf(comment, 256, "argv[%d][%d]", i, j);
				//msg("Converting memory to symbolic " HEX_FORMAT "\n", current_argv + j);
				triton::api.convertMemoryToSymbolicVariable(triton::arch::MemoryAccess(current_argv + j*char_size, char_size, current_char), comment);
			}
			j++;
		} while (memcmp(&current_char, null_byte, char_size) != 0);
		if (j > 1)
		{
			//Enable trigger, something is tainted...
			start_tainting_or_symbolic_analysis();
			if (cmdOptions.showDebugInfo)
				msg("[!] argv[%d] %s (%d bytes)\n", i, cmdOptions.use_tainting_engine ? "Tainted" : "Symbolized", j);
		}
	}
}

/*This function set all the breakpoints to automatically taint all the user inputs: argc, argv, recv, fread, etc..*/
void set_automatic_taint_n_simbolic()
{
	if (cmdOptions.taintArgv)
	{
		//We should transparentelly hook the main so we could taint the argv when the execution is there
		//First we need to find the main function. Look first for wmain since if wmain exists main is more likely to exist too
		//Adding wmain feature https://github.com/illera88/Ponce/issues/56
		ea_t main_function = find_function("wmain");
		if (main_function == -1)
		{
			//Maybe we should look for more? _tmain?
			main_function = find_function("_wmain");
			if (main_function == -1)
			{
				// Lets search for wmain
				main_function = find_function("main");
				if (main_function == -1)
				{
					main_function = find_function("_main");
					if (main_function == -1){
						msg("[!] main function not found, we cannot taint the args :S\n");
						return;
					}
				}	
			}
		}
		if (cmdOptions.showDebugInfo) {
			if (inf.is_64bit())
				msg("[+] main function found at %#llx\n", main_function);
			else
				msg("[+] main function found at %#x\n", main_function);

		}
		//Then we should check if there is already a breakpoint
		bpt_t breakpoint;
		bool bp_exists = get_bpt(main_function, &breakpoint);
		//Then we should add a callback to do at that breakpoint, we should indicate if there was a previous bp there	
		//if not we should continue the execution without stop
		breakpoint_pending_action bpa;
		bpa.address = main_function;
		bpa.ignore_breakpoint = bp_exists;
		bpa.callback = taint_or_symbolize_main_callback;
		//We add the action to the list
		breakpoint_pending_actions.push_back(bpa);
		if (!bp_exists)
		{
			//We need to add a breakpoint
			add_bpt(main_function, 0, BPT_SOFT);
		}
	}
}

/*This function gets the tainted operands for an instruction and add a comment to that instruction with this info*/
void get_controlled_operands_and_add_comment(triton::arch::Instruction* tritonInst, ea_t pc)//, std::list<triton::arch::OperandWrapper> &tainted_reg_operands)
{
	std::stringstream comment;
	std::stringstream regs_controlled;
	std::stringstream mems_controlled;

	/*Here we check all the registers and memory read to know which are tainted*/
	auto regs = tritonInst->getReadRegisters();
	for (auto it = regs.begin(); it != regs.end(); it++)
	{
		auto reg = it->first;
		if ((cmdOptions.use_tainting_engine && triton::api.isRegisterTainted(reg)) ||
			(cmdOptions.use_symbolic_engine && triton::api.getSymbolicRegisterId(reg) != triton::engines::symbolic::UNSET && triton::api.getSymbolicExpressionFromId(triton::api.getSymbolicRegisterId(reg))->isSymbolized()))
			regs_controlled << reg.getName() << " ";
	}
	if (regs_controlled.str().size() > 0)
	{
		if (cmdOptions.use_tainting_engine)
			comment << "Tainted regs: " << regs_controlled.str();
		else
			comment << "Symbolized regs: " << regs_controlled.str();
	}
	auto accesses = tritonInst->getLoadAccess();
	for (auto it = accesses.begin(); it != accesses.end(); it++)
	{
		auto mem = it->first;
		//For the memory we can't use the operand because they don't have yet the real value of the address
		if ((cmdOptions.use_tainting_engine && triton::api.isMemoryTainted(mem)) ||
			(cmdOptions.use_symbolic_engine && triton::api.getSymbolicMemoryId(mem.getAddress()) != triton::engines::symbolic::UNSET && triton::api.getSymbolicExpressionFromId(triton::api.getSymbolicMemoryId(mem.getAddress()))->isSymbolized()))
			mems_controlled << "0x" << std::hex << mem.getAddress() << " ";
	}
	if (mems_controlled.str().size() > 0)
	{
		if (cmdOptions.use_tainting_engine)
			comment << "Tainted memory: " << mems_controlled.str();
		else
			comment << "Symbolized memory: " << mems_controlled.str();
	}
		
	//We set the comment
	if (comment.str().size() > 0)
	{
		set_cmt(pc, comment.str().c_str(), false);
	}
}
