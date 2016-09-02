#include <list>
// Ponce
#include "callbacks.hpp"
#include "globals.hpp"
#include "context.hpp"
#include "utils.hpp"
#include "tainting.hpp"

//IDA
#include <ida.hpp>
#include <dbg.hpp>
#include <loader.hpp>
#include <intel.hpp>

//Triton
#include "api.hpp"

std::list<breakpoint_pending_action> breakpoint_pending_actions;

/*This function will create and fill the Triton object for every instruction*/
void tritonize(ea_t pc, thid_t threadID)
{
	/*Check tha the runtime Trigger is on just in case*/
	if (!runtimeTrigger.getState())
		return;

	//We delete the last_instruction
	if (last_triton_instruction != NULL)
		delete last_triton_instruction;
	triton::arch::Instruction* tritonInst = new triton::arch::Instruction();
	last_triton_instruction = tritonInst;
	//ea_t pc = va_arg(va, ea_t);
	/*This will fill the 'cmd' (to get the instruction size) which is a insn_t structure https://www.hex-rays.com/products/ida/support/sdkdoc/classinsn__t.html */
	if (!decode_insn(pc))
		warning("[!] Some error decoding instruction at %p", pc);	
	
	//thid_t threadID = va_arg(va, thid_t);
	/*char buf[50];
	ua_mnem(pc, buf, sizeof(buf));*/
	if (DEBUG)
		msg("[+] Triton At "HEX_FORMAT" thread id: %d\n", pc, threadID);
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

	/*std::list<triton::arch::OperandWrapper> tainted_reg_operands;
	if (ADD_COMMENTS_WITH_TAINTING_INFORMATION)
		tainted_reg_operands = get_tainted_regs_operands(tritonInst);*/

	/* Process the IR and taint */
	triton::api.buildSemantics(*tritonInst);

	if (ADD_COMMENTS_WITH_TAINTING_INFORMATION)
		get_tainted_operands_and_add_comment(tritonInst, pc);// , tainted_reg_operands);

	/* Trust operands */
	for (auto op = tritonInst->operands.begin(); op != tritonInst->operands.end(); op++)
		op->setTrust(true);


	if (tritonInst->isTainted())
	{
		if (DEBUG)
			msg("[!] Instruction tainted at "HEX_FORMAT"\n", pc);
		if (RENAME_TAINTED_FUNCTIONS)
			rename_tainted_function(pc);
		if (tritonInst->isBranch()) // Check if it is a conditional jump
			set_item_color(pc, COLOR_TAINTED_CONDITION);
		else
			set_item_color(pc, COLOR_TAINTED);
	}
}

/*This function is called when we taint a register that is used in the current instruction*/
void reanalize_current_instruction()
{
	if (DEBUG)
		msg("Reanalizyng instruction at "HEX_FORMAT"\n");
	uint64 eip;
	get_reg_val("eip", &eip);
	tritonize((triton::__uint)eip, get_current_thread());
}

/*This functions is called every time a new debugger session starts*/
void triton_restart_engines()
{
	if (DEBUG)
		msg("[+] Restarting triton engines...\n");
	//We reset everything at the beginning
	triton::api.resetEngines();
	//We disable the tainting engine if the user doesn't want to use it
	if (!ENABLE_TAINTING_ENGINE)
		triton::api.getTaintEngine()->enable(false);
	//We disable the symbolic engine if the user doesn't want to use it
	if (!ENABLE_SYMBOLIC_ENGINE)
		triton::api.getSymbolicEngine()->enable(false);
	runtimeTrigger.disable();
	is_something_tainted = false;
	tainted_functions_index = 0;
	//Reset instruction counter
	total_number_traced_ins = current_trace_counter = 0;
	breakpoint_pending_actions.clear();
	set_automatic_tainting();
}

int idaapi tracer_callback(void *user_data, int notification_code, va_list va)
{
	//msg("Notification code:%d\n",notification_code);
	switch (notification_code)
	{
		case dbg_process_start:
		{
			if (DEBUG)
				msg("[+] Starting the debugged process. Reseting all the engines.\n");
			triton_restart_engines();
			break;
		}
		case dbg_step_into:
		case dbg_step_over:
		{
			//If tracing is enable for each one of this event is launched another dbg_trace. So we should ignore this one
			if (ENABLE_TRACING_WHEN_TAINTING)
				break;
			//We want to enable the user to do step into/over, so he could choose whitch functions skip and with conditions negate
			debug_event_t* debug_event = va_arg(va, debug_event_t*);
			thid_t tid = debug_event->tid;
			ea_t pc = debug_event->ea;
			if (EXTRADEBUG)
				msg("[+] Stepping %s: "HEX_FORMAT" (Tid: %d)\n", notification_code == dbg_step_into ? "into" : "over", pc, tid);
			if (PAINT_EXECUTED_INSTRUCTIONS)
				set_item_color(pc, COLOR_EXECUTED_INSTRUCTION);
			tritonize(pc, tid);
			break;
		}
		case dbg_trace:
		{
			// A step occured (one instruction was executed). This event
			// notification is only generated if step tracing is enabled.
			msg("dbg_trace\n");
			//Create the triton instance for the Instruction

			thid_t tid = va_arg(va, thid_t);
			ea_t pc = va_arg(va, ea_t);

			//msg("[%d] tracing over: "HEX_FORMAT"\n", g_nb_insn, pc);
			if (PAINT_EXECUTED_INSTRUCTIONS)
				set_item_color(pc, COLOR_EXECUTED_INSTRUCTION);
			tritonize(pc, tid);

			current_trace_counter++;
			total_number_traced_ins++;

			if (limit_traced_instructions && current_trace_counter == max_traced_instructions)
			{
				int answer = askyn_c(1, "[?] %d instructions has been traced. Do you want to execute %d more?", total_number_traced_ins, max_traced_instructions);
				if (answer == 0 || answer == -1) //No or Cancel
				{
					// stop the trace mode and suspend the process
					disable_step_trace();
					suspend_process();
					msg("[!] Process suspended (Traced %d instructions)\n", total_number_traced_ins);
				}
				else
				{
					current_trace_counter = 0;
				}
			}
			break;
		}
		case dbg_bpt:
		{
			msg("dbg_bpt\n");
			thid_t tid = va_arg(va, thid_t);
			ea_t pc = va_arg(va, ea_t);
			int *warn = va_arg(va, int *);
			//msg("Breakpoint reached! At "HEX_FORMAT"\n", pc);
			//We look if there is a pending action for this breakpoint
			for (auto it = breakpoint_pending_actions.begin(); it != breakpoint_pending_actions.end(); ++it)
			{
				breakpoint_pending_action bpa = *it;
				//If we find a pendign action we execute the callback
				if (pc == bpa.address)
				{
					bpa.callback(pc);
					if (PAINT_EXECUTED_INSTRUCTIONS)
						set_item_color(pc, COLOR_EXECUTED_INSTRUCTION);
					tritonize(pc, tid);
					//If there is a user-defined bp in the same address we should respect it and dont continue the exec
					if (!bpa.ignore_breakpoint)
					{
						//If not this is the bp we set to taint the arguments, we should rmeove it and continue the execution
						del_bpt(pc);
						continue_process();
					}
					else
					{

					}
					break;
				}
			}
			break;
		}
		case dbg_process_exit:
		{
			if (DEBUG)
				msg("[!] Process_exiting...\n");
			if (ENABLE_TRACING_WHEN_TAINTING)
			{
				if (DEBUG)
					msg("[+] Clearing trace...\n");
				clear_trace();
			}
			//msg("In dbg_process_exit, reseting everything\n");
			//Do we want to unhook this event?
			//unhook_from_notification_point(HT_DBG, tracer_callback, NULL);
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
		case ui_populating_tform_popup:
		{
			TForm *form = va_arg(va, TForm *);
			TPopupMenu *popup_handle = va_arg(va, TPopupMenu *);
			int view_type= get_tform_type(form);

			/*Iterate over all the actions*/
			for (int i = 0;; i++)
			{
				if (action_list[i].name == NULL)
					break;
			
				/*Iterate over the view types of every action*/
				for (int j=0;; j++)
				{
					if (action_list[i].view_type[j] == NULL){
						break;
					}
					if (action_list[i].view_type[j] == view_type){
						attach_action_to_popup(form, popup_handle, action_list[i].name, NULL, SETMENU_FIRST);

					}
				}	
			}
		}
		case dbg_process_exit:{
			unhook_from_notification_point(HT_DBG, ui_callback, NULL);
			break;
		}
	}
	return 0;
}

