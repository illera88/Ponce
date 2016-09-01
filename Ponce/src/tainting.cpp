//Ponce
#include "callbacks.hpp"
#include "globals.hpp"
#include "utils.hpp"

//IDA
#include <ida.hpp>
#include <dbg.hpp>

//Triton


void taint_main_callback(ea_t main_address)
{
	//We need to invalidate the memory to forze IDA to reaload all the segments and new allocs
	invalidate_dbgmem_config();

	//Iterate through argc, argv[argc] and taint every byte and argc
	triton::__uint argc = get_args(0, true);
	triton::__uint argv = get_args(1, true);
	//msg("argc: %d argv: "HEX_FORMAT"\n", argc, argv);
	if (TAINT_ARGC)
	{
		//First we taint the argc
#ifdef X86_32
		//In x86 we taint the memory of the first arg, argc
		triton::api.taintMemory(triton::arch::MemoryAccess(get_args_pointer(0, true), 4, argc));
		if (DEBUG)
			msg("[!] argc tainted\n");
#elif 
		//Inx64 we taint the register rcx
		auto reg = str_to_register("RCX");
		reg.setConcreteValue(argc);
		triton::api.taintRegister(reg);
#endif
		start_tainting_analysis();
	}
	//Second we taint all the arguments values
	//We are tainting the argv[0], this is the program path, and it is something that the 
	//user controls and sometimes is used to do somechecks
	for (unsigned int i = SKIP_ARGV0; i < argc; i++)
	{
		triton::__uint current_argv = read_uint_from_ida(argv + i * REG_SIZE);
		if (current_argv == 0xffffffff)
		{
			msg("[!] Error reading mem~ "HEX_FORMAT"\n", argv + i * REG_SIZE);
			break;
		}
		//We iterate through all the bytes of the current argument
		int j = 0;
		char current_char;
		do
		{
			current_char = read_char_from_ida(current_argv + j);
			if (current_char == '\0' && !TAINT_END_OF_STRING)
				break;
			if (EXTRADEBUG)
				msg("[!] Tainting argv[%d][%d]: %c\n", i, j, current_char);
			triton::api.taintMemory(triton::arch::MemoryAccess(current_argv + j, 1, current_char));
			j++;
		} while (current_char != '\0');
		if (j > 1)
		{
			//Enable trigger, something is tainted...
			start_tainting_analysis();
			if (DEBUG)
				msg("[!] argv[%d] tainted (%d bytes)\n", i, j);
		}
	}
}

/*This function set all the breakpoints to automatically taint all the user inputs: argc, argv, recv, fread, etc..*/
void set_automatic_tainting()
{
	if (TAINT_ARGV)
	{
		//We should transparentelly hook the main so we could taint the argv when the execution is there
		//First we need to find the main function
		ea_t main_function = find_function("main");
		if (main_function == -1)
		{
			//Maybe we should look for more? _tmain?
			main_function = find_function("_main");
			if (main_function == -1)
			{
				msg("[!] main function not found, we cannot taint the args :S\n");
				return;
			}
		}
		if (DEBUG)
			msg("[+] main function found at "HEX_FORMAT"\n", main_function);
		//Then we should check if there is already a breakpoint
		bpt_t breakpoint;
		bool bp_exists = get_bpt(main_function, &breakpoint);
		//Then we should add a callback to do at that breakpoint, we should indicate if there was a previous bp there	
		//if not we should continue the execution without stop
		breakpoint_pending_action bpa;
		bpa.address = main_function;
		bpa.ignore_breakpoint = bp_exists;
		bpa.callback = taint_main_callback;
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
void get_tainted_operands_and_add_comment(triton::arch::Instruction* tritonInst, ea_t pc)
{
	//ToDo: Externalize this to another function
	std::stringstream comment;
	/*Here we check all the registers and memory read to know which are tainted*/
	auto regs = tritonInst->getReadRegisters();
	for (auto it = regs.begin(); it != regs.end(); it++)
	{
		auto reg = it->first;
		if (triton::api.isRegisterTainted(reg))
			comment << "Reg " << reg.getName() << " is tainted ";
	}
	auto accesses = tritonInst->getLoadAccess();
	for (auto it = accesses.begin(); it != accesses.end(); it++)
	{
		auto mem = it->first;
		if (triton::api.isMemoryTainted(mem))
			comment << "Mem 0x" << std::hex << mem.getAddress() << " is tainted ";
	}
	//We set the comment
	if (comment.str().size() > 0)
	{
		set_cmt(pc, comment.str().c_str(), false);
	}
}