#include <list>
// Ponce
#include "callbacks.hpp"
#include "globals.hpp"
#include "context.hpp"
#include "utils.hpp"
#include "tainting_n_symbolic.hpp"
#include "blacklisted.hpp"
#include "actions.hpp"

//IDA
#include <ida.hpp>
#include <dbg.hpp>
#include <loader.hpp>
#include <intel.hpp>

//Triton
#include "api.hpp"
#include "x86Specifications.hpp"

std::list<breakpoint_pending_action> breakpoint_pending_actions;

/*This function will create and fill the Triton object for every instruction*/
void tritonize(ea_t pc, thid_t threadID)
{
	/*Check tha the runtime Trigger is on just in case*/
	if (!ponce_runtime_status.runtimeTrigger.getState())
		return;

	//We delete the last_instruction
	//Maybe in the future we need to keep the in instruction memory to negate the condition at any moment
	if (ponce_runtime_status.last_triton_instruction != NULL)
		delete ponce_runtime_status.last_triton_instruction;
	triton::arch::Instruction* tritonInst = new triton::arch::Instruction();
	ponce_runtime_status.last_triton_instruction = tritonInst;
	//ea_t pc = va_arg(va, ea_t);
	/*This will fill the 'cmd' (to get the instruction size) which is a insn_t structure https://www.hex-rays.com/products/ida/support/sdkdoc/classinsn__t.html */
	if (!decode_insn(pc))
		warning("[!] Some error decoding instruction at %p", pc);	
	
	//thid_t threadID = va_arg(va, thid_t);
	/*char buf[50];
	ua_mnem(pc, buf, sizeof(buf));*/
	unsigned char opcodes[15];
	get_many_bytes(pc, opcodes, sizeof(opcodes));

	/* Setup Triton information */
	tritonInst->partialReset();
	tritonInst->setOpcodes((triton::uint8*)opcodes, cmd.size);
	tritonInst->setAddress(pc);
	tritonInst->setThreadId(threadID);

	/* Disassemble the instruction */
	try{
		triton::api.disassembly(*tritonInst);
	}
	catch (...){
		msg("[!] Dissasembling error at "HEX_FORMAT" Opcodes:",pc);
		for (auto i = 0; i < cmd.size; i++)
			msg("%2x ", *(unsigned char*)(opcodes + i));
		msg("\n");
		return;
	}
	if (cmdOptions.showDebugInfo)
		msg("[+] Triton At "HEX_FORMAT": %s (Thread id: %d)\n", pc, tritonInst->getDisassembly().c_str(), threadID);

	/*std::list<triton::arch::OperandWrapper> tainted_reg_operands;
	if (ADD_COMMENTS_WITH_TAINTING_INFORMATION)
		tainted_reg_operands = get_tainted_regs_operands(tritonInst);*/

	/* Process the IR and taint */
	triton::api.buildSemantics(*tritonInst);

	/*In the case that the snapshot engine is in use we shoudl track every memory write access*/
	if (snapshot.exists()){
		auto store_access_list = tritonInst->getStoreAccess();
		for (auto it = store_access_list.begin(); it != store_access_list.end(); it++)
		{
			//Possible point of failure since memory_access can be more than one byte i guess
			triton::arch::MemoryAccess memory_access = it->first;
			auto addr = memory_access.getAddress();
			//This is the way to force IDA to read the value from the debugger
			//More info here: https://www.hex-rays.com/products/ida/support/sdkdoc/dbg_8hpp.html#ac67a564945a2c1721691aa2f657a908c
			invalidate_dbgmem_contents((ea_t)addr, memory_access.getSize()); //ToDo: Do I have to call this for every byte in memory I want to read?
			for (unsigned int i = 0; i < memory_access.getSize(); i++){
				triton::uint128 value = 0;
				get_many_bytes((ea_t)addr+i, &value, 1);
				snapshot.addModification((ea_t)addr + i, value.convert_to<char>());
			}
		}
	}

	if (cmdOptions.addCommentsControlledOperands)
		get_controlled_operands_and_add_comment(tritonInst, pc);// , tainted_reg_operands);

	if (cmdOptions.addCommentsSymbolicExpresions)
		add_symbolic_expressions(tritonInst, pc);

	/* Trust operands */
	for (auto op = tritonInst->operands.begin(); op != tritonInst->operands.end(); op++)
		op->setTrust(true);

	if (cmdOptions.paintExecutedInstructions)
		set_item_color(pc, cmdOptions.color_executed_instruction);

	//ToDo: The isSymbolized is missidentifying like "user-controlled" some instructions: https://github.com/JonathanSalwan/Triton/issues/383
	if (tritonInst->isTainted() || tritonInst->isSymbolized())
	{
		if (cmdOptions.showDebugInfo)
			msg("[!] Instruction %s at "HEX_FORMAT"\n", tritonInst->isTainted()? "tainted": "symbolized", pc);
		if (cmdOptions.RenameTaintedFunctionNames)
			rename_tainted_function(pc);
		if (tritonInst->isBranch()) // Check if it is a conditional jump
			set_item_color(pc, cmdOptions.color_tainted_condition);
		else
			set_item_color(pc, cmdOptions.color_tainted);
	}

	if (tritonInst->isBranch() && tritonInst->isSymbolized())
	{
		triton::__uint addr1 = (triton::__uint)tritonInst->getNextAddress();
		triton::__uint addr2 = (triton::__uint)tritonInst->operands[0].getImmediate().getValue();
		if (cmdOptions.showDebugInfo)
			msg("[+] Branch symbolized detected at "HEX_FORMAT": "HEX_FORMAT" or "HEX_FORMAT", Taken:%s\n", pc, addr1, addr2, tritonInst->isConditionTaken() ? "Yes" : "No");
		triton::__uint ripId = triton::api.getSymbolicRegisterId(TRITON_X86_REG_PC);
		if (tritonInst->isConditionTaken())
			ponce_runtime_status.myPathConstraints.push_back(PathConstraint(ripId, pc, addr2, addr1, ponce_runtime_status.myPathConstraints.size()));
		else
			ponce_runtime_status.myPathConstraints.push_back(PathConstraint(ripId, pc, addr1, addr2, ponce_runtime_status.myPathConstraints.size()));
	}
	//We add the instruction to the map, so we can use it later to negate conditions, view SE, slicing, etc..
	//instructions_executed_map[pc].push_back(tritonInst);
}

/*This function is called when we taint a register that is used in the current instruction*/
void reanalize_current_instruction()
{
	if (cmdOptions.showDebugInfo)
		msg("Reanalizyng instruction at "HEX_FORMAT"\n");
	uint64 eip;
	get_reg_val("eip", &eip);
	tritonize((triton::__uint)eip, get_current_thread());
}

/*This functions is called every time a new debugger session starts*/
void triton_restart_engines()
{
	if (cmdOptions.showDebugInfo)
		msg("[+] Restarting triton engines...\n");
	//We reset everything at the beginning
	triton::api.resetEngines();
	//If we are in taint analysis mode we enable only the tainting engine and disable the symbolic one
	triton::api.getTaintEngine()->enable(cmdOptions.use_tainting_engine);
	triton::api.getSymbolicEngine()->enable(cmdOptions.use_symbolic_engine);
	//triton::api.getSymbolicEngine()->enable(true);
	ponce_runtime_status.runtimeTrigger.disable();
	ponce_runtime_status.is_something_tainted_or_symbolize = false;
	ponce_runtime_status.tainted_functions_index = 0;
	//Reset instruction counter
	ponce_runtime_status.total_number_traced_ins = ponce_runtime_status.current_trace_counter = 0;
	breakpoint_pending_actions.clear();
	set_automatic_taint_n_simbolic();
	ponce_runtime_status.myPathConstraints.clear();
}

int idaapi tracer_callback(void *user_data, int notification_code, va_list va)
{
	msg("Notification code: %d str: %s\n",notification_code, notification_code_to_string(notification_code).c_str());
	switch (notification_code)
	{
		case dbg_process_start:
		{
			if (cmdOptions.showDebugInfo)
				msg("[+] Starting the debugged process. Reseting all the engines.\n");
			triton_restart_engines();
			clear_requests_queue();
			break;
		}
		case dbg_step_into:
		case dbg_step_over:
		{
			//If the trigger is disbaled then the user is manually stepping with the ponce tracing disabled
			if (!ponce_runtime_status.runtimeTrigger.getState())
				break;
			//msg("dbg_step_?\n");
			//If tracing is enable for each one of this event is launched another dbg_trace. So we should ignore this one
			/*if (ENABLE_TRACING_WHEN_TAINTING)
				break;*/
			//We want to enable the user to do step into/over, so he could choose whitch functions skip and with conditions negate
			debug_event_t* debug_event = va_arg(va, debug_event_t*);
			thid_t tid = debug_event->tid;
			ea_t pc = debug_event->ea;

			if (!decode_insn(pc))
				warning("[!] Some error decoding instruction at %p", pc);
			
			//msg("dbg_step_? at "HEX_FORMAT"\n", pc);
			//We need to check if the instruction has been analyzed already. This happens when we are stepping into/over and 
			//we find a breakpoint we set (main, recv, fread), we are receiving two events: dbg_bpt and dbg_step_into for the 
			//same instruction. And we want to tritonize in dbg_bpt for example when we put bp in main and we execute the program
			if (ponce_runtime_status.last_triton_instruction != NULL && ponce_runtime_status.last_triton_instruction->getAddress() != pc)
			{
				if (cmdOptions.showExtraDebugInfo)
					msg("[+] Stepping %s: "HEX_FORMAT" (Tid: %d)\n", notification_code == dbg_step_into ? "into" : "over", pc, tid);
				tritonize(pc, tid);
			}
			/*else
			{
				if (last_triton_instruction == NULL)
					msg("last_triton)isntructionn NULL\n");
				else
					msg("last_triton_instruction->getAddress(): "HEX_FORMAT"\n", last_triton_instruction->getAddress());
			}*/
			//Continue stepping
			//msg("automatically_continue_after_step: %d\n", automatically_continue_after_step);
			/*if (automatically_continue_after_step)
			{
				//This is the wow64 switching, we need to skip it
				if (last_triton_instruction->getDisassembly().find("call dword ptr fs:[0xc0]") != -1)
				{
					msg("wow64 switching! request_step_over();\n");
					request_step_over();
				}
				else// if (notification_code == dbg_step_into)
				{
					msg("dbg_step request_step_into();\n");
					request_step_into();
				}
				/*else
				{
					msg("dbg_step request_step_over();\n");
					request_step_over();
				}*/
			//}
			break;
		}
		case dbg_trace:
		{
			//If the trigger is disbaled then the user is manually stepping with the ponce tracing disabled
			if (!ponce_runtime_status.runtimeTrigger.getState())
				break;
			// A step occured (one instruction was executed). This event
			// notification is only generated if step tracing is enabled.
			//msg("dbg_trace\n");
			//Create the triton instance for the Instruction

			thid_t tid = va_arg(va, thid_t);
			ea_t pc = va_arg(va, ea_t);

			//Sometimes the cmd structure doesn't correspond with the traced instruction
			//With this we are filling cmd with the instruction at the address specified
			ua_ana0(pc);
			//msg("cmd.ea: "HEX_FORMAT"\n", cmd.ea);
			// We do this to blacklist API that does not change the tainted input
			if (cmd.itype == NN_call || cmd.itype == NN_callfi || cmd.itype == NN_callni)
			{
				bool need_to_break = false;
				qstring callee = get_callee(pc);
				unsigned int number_items = sizeof(black_func) / sizeof(char *);
				for (unsigned int i = 0; i < number_items; i++)
				{
					//warning("%s<->%s\n", callee.c_str(), black_func[i]);
					if (strcmp(callee.c_str(), black_func[i]) == 0)
					{
						//warning("call blacklisted");
						//We are in a call to a blacklisted function.
						/*We should set a BP in the next instruction right after the
						blacklisted callback to enable tracing again*/
						ea_t next_ea = next_head(pc, BADADDR);
						//warning("Set bp here "HEX_FORMAT"\n", next_ea);
						add_bpt(next_ea, 1, BPT_EXEC);

						breakpoint_pending_action bpa;
						bpa.address = next_ea;
						bpa.ignore_breakpoint = false;
						bpa.remove_after_found = true; // we remove this structure from breakpoint_pending_actions after it gets reached
						bpa.callback = enableTrigger; // We will enable back the trigger when this bp get's reached
						//We add the action to the list
						breakpoint_pending_actions.push_back(bpa);

						//Disabling step tracing...
						disable_step_trace();
						ponce_runtime_status.runtimeTrigger.disable();
						need_to_break = true;
						break;
					}
				}
				//We don't want to tritonize this call
				if (need_to_break)
					break;
			}

			//msg("Tracing over: "HEX_FORMAT"\n", pc);
			tritonize(pc, tid);

			ponce_runtime_status.current_trace_counter++;
			ponce_runtime_status.total_number_traced_ins++;

			//This is the wow64 switching, we need to skip it
			if (ponce_runtime_status.last_triton_instruction->getDisassembly().find("call dword ptr fs:[0xc0]") != -1)
			{
				msg("wow64 switching! requesting disable();\n");
				//And now we need to stop the tracing, do step over and reenable the tracing...
				//disable_step_trace();
				suspend_process();
				request_step_over();
				request_continue_process();
				run_requests();
				break;
			}

			if (cmdOptions.limitInstructionsTracingMode && ponce_runtime_status.current_trace_counter == cmdOptions.limitInstructionsTracingMode)
			{
				int answer = askyn_c(1, "[?] %d instructions has been traced. Do you want to execute %d more?", ponce_runtime_status.total_number_traced_ins, cmdOptions.limitInstructionsTracingMode);
				if (answer == 0 || answer == -1) //No or Cancel
				{
					// stop the trace mode and suspend the process
					enable_step_trace(false);
					suspend_process();
					msg("[!] Process suspended (Traced %d instructions)\n", ponce_runtime_status.total_number_traced_ins);
				}
				else
				{
					ponce_runtime_status.current_trace_counter = 0;
				}
			}
			break;
		}
		case dbg_bpt:
		{
			thid_t tid = va_arg(va, thid_t);
			ea_t pc = va_arg(va, ea_t);
			int *warn = va_arg(va, int *);
			//msg("dbg_bpt at "HEX_FORMAT"\n", pc);
			//This variable defines if a breakpoint is a user-defined breakpoint or not
			bool user_bp = true;
			msg("Breakpoint reached! At "HEX_FORMAT"\n", pc);
			//We look if there is a pending action for this breakpoint
			for (auto it = breakpoint_pending_actions.begin(); it != breakpoint_pending_actions.end(); ++it)
			{
				breakpoint_pending_action bpa = *it;
				//If we find a pendign action we execute the callback
				if (pc == bpa.address)
				{
					msg("breakpoint pending found\n");
					bpa.callback(pc);
					tritonize(pc, tid);
					//If there is a user-defined bp in the same address we should respect it and dont continue the exec
					if (!bpa.ignore_breakpoint)
					{
						//If it's a breakpoint the plugin set not a user-defined bp
						user_bp = false;
						//If not this is the bp we set to taint the arguments, we should rmeove it and continue the execution
						del_bpt(pc);
						enable_step_trace(true);
						//We dont want to skip library funcions or debug segments
						set_step_trace_options(0);
						continue_process();

						if (bpa.remove_after_found)
							breakpoint_pending_actions.erase(it);
					}
					break;
				}
			}
			//If it is a user break point we enable again the step tracing if it was enabled previously...
			//The idea is if the user uses Execute native til next bp, and IDA reachs the next bp we reenable the tracing
			if (user_bp)
			{
				//If the trigger is disabled then the user is manually stepping with the ponce tracing disabled
				if (ponce_runtime_status.runtimeTrigger.getState())
					enable_step_trace(ponce_runtime_status.runtimeTrigger.getState());
			}
			break;
		}
		case dbg_process_exit:
		{
			if (cmdOptions.showDebugInfo)
				msg("[!] Process_exiting...\n");
			/*if (ENABLE_TRACING_WHEN_TAINTING)
			{
				if (DEBUG)
					msg("[+] Clearing trace...\n");
				clear_trace();
			}*/
			//msg("In dbg_process_exit, reseting everything\n");
			//Do we want to unhook this event?
			//unhook_from_notification_point(HT_DBG, tracer_callback, NULL);
			ponce_runtime_status.runtimeTrigger.disable();
			break;
		}
	}
	return 0;
}

//---------------------------------------------------------------------------
// Callback for ui notifications
int idaapi ui_callback(void * ud, int notification_code, va_list va)
{
	switch (notification_code)
	{
		// called when IDA is preparing a context menu for a view
		// Here dynamic context-depending user menu items can be added.
		case ui_finish_populating_tform_popup:
		{
			TForm *form = va_arg(va, TForm *);
			TPopupMenu *popup_handle = va_arg(va, TPopupMenu *);
			int view_type= get_tform_type(form);

			//Adding a separator
			attach_action_to_popup(form, popup_handle, "");

			/*Iterate over all the actions*/			
			for (int i = 0;; i++)
			{
				if (action_list[i].action_decs == NULL)
					break;
			
				/*Iterate over the view types of every action*/
				for (int j=0;; j++)
				{
					if (action_list[i].view_type[j] == NULL){
						break;
					}
					if (action_list[i].view_type[j] == view_type)
					{
						//We only attach to the popup if the action makes sense with the current configuration
						if (cmdOptions.use_tainting_engine && action_list[i].enable_taint || cmdOptions.use_symbolic_engine && action_list[i].enable_symbolic)
						{
							attach_action_to_popup(form, popup_handle, action_list[i].action_decs->name, action_list[i].menu_path, SETMENU_APP);
						}
					}
				}	
			}
			//Adding submenus for solve with all the conditions
			/*if (view_type == BWN_DISASM)
			{
				for (unsigned int i = 0; i < ponce_runtime_status.myPathConstraints.size(); i++)
				{
					//We should filter here for the pc
					char name[256];
					sprintf_s(name, "Ponce:solve_formula_sub_%d", i);
					action_IDA_solve_formula_sub.name = name;
					char label[256];
					sprintf_s(label, "%d. "HEX_FORMAT" -> %s ", ponce_runtime_status.myPathConstraints[i].bound, ponce_runtime_status.myPathConstraints[i].conditionAddr, ponce_runtime_status.myPathConstraints[i].takenAddr);
					action_IDA_solve_formula_sub.label = label;
					bool success = register_action(action_IDA_solve_formula_sub);
					msg("registered submenu for solver, result: %d\n", success);
					success = attach_action_to_popup(form, popup_handle, action_IDA_solve_formula_sub.name, "SMT/Solve formula/", SETMENU_APP);
					msg("Added submenu for solver, result: %d\n", success);
				}
			}*/
			//Adding a separator
			attach_action_to_popup(form, popup_handle, "");
		}
		case dbg_process_exit:
		{
			unhook_from_notification_point(HT_DBG, ui_callback, NULL);
			break;
		}
	}
	return 0;
}

/*We set the memory to the results we got and do the analysis from there*/
void set_SMT_results(Input *input_ptr){
	if (cmdOptions.showDebugInfo)
		msg("\ncallback_set_SMT_results \n");

	/*To set the memory types*/
	for (auto it = input_ptr->memOperand.begin(); it != input_ptr->memOperand.end(); it++)
		put_many_bytes((ea_t)it->getAddress(), &it->getConcreteValue(), it->getSize());	
		
	/*To set the register types*/
	for (auto it = input_ptr->regOperand.begin(); it != input_ptr->regOperand.end(); it++)
		set_reg_val(it->getName().c_str(), it->getConcreteValue().convert_to<uint64>());
		
	if (cmdOptions.showDebugInfo)
		msg("[+] Memory set with the SMT results\n");
}
