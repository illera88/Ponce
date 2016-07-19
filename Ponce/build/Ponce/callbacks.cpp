
// Ponce

#include "callbacks.hpp"
#include "globals.hpp"
#include "memoryinstrumentation.hpp"


//IDA
#include <ida.hpp>
#include <dbg.hpp>
#include <loader.hpp>
#include <intel.hpp>




//Triton
#include "api.hpp"

//DR
//#include "dr_api.h"

/*This function will create and fill the Triton object for every instruction*/
void tritonize(ea_t pc, thid_t threadID)
{
	/*Check tha the runtime Trigger is on just in case*/
	if (!runtimeTrigger.getState())
		return;

	using namespace triton;
	arch::Instruction* tritonInst = new arch::Instruction();

	//ea_t pc = va_arg(va, ea_t);
	/*This will fill the 'cmd' (to get the instruction size) which is a insn_t structure https://www.hex-rays.com/products/ida/support/sdkdoc/classinsn__t.html */
	if (!decode_insn(pc))
		warning("[!] Some error decoding instruction at %p", pc);	
	
	//thid_t threadID = va_arg(va, thid_t);
	char buf[50];
	ua_mnem(pc, buf, sizeof(buf));
	msg("At "HEX_FORMAT" %s size %d thread id: %d\n", pc, buf, cmd.size, threadID);
	unsigned char opcodes[15];
	get_many_bytes(pc, opcodes, sizeof(opcodes));

	///* Setup Triton information */
	tritonInst->partialReset();
	tritonInst->setOpcodes((triton::uint8*)opcodes, cmd.size);
	tritonInst->setAddress(pc);
	tritonInst->setThreadId(threadID);

	/* Disassemble the instruction */
	try{
		api.disassembly(*tritonInst);
	}
	catch (...){
		msg("[-]Dissasembling error at "HEX_FORMAT" Opcodes:",pc);
		for (auto i = 0; i < cmd.size; i++)
			msg("%2x ", *(unsigned char*)(opcodes + i));
		msg("\n");
		return;
	}

	/* Trust operands */
	for (auto op = tritonInst->operands.begin(); op != tritonInst->operands.end(); op++)
		op->setTrust(true);

	/* Process the IR and taint */
	api.buildSemantics(*tritonInst);

	if (tritonInst->isTainted())
	{
		msg("Estoy tintado\n");
		if(insn_jcc()) // Check if it is a conditional jump
			set_item_color(pc, COLOR_TAINTED_JUMP);
		else
			set_item_color(pc, COLOR_TAINTED);
	}

	/*We need to extract the memory that has been read from or write to*/
	//instrumentMemoryAccess(opcodes, tritonInst);


}


int idaapi tracer_callback(void * /*user_data*/, int notification_code, va_list va)
{
	/*if (g_nb_insn == 0)
	myfunc();*/
	msg("Notification code:%d\n",notification_code);
	switch (notification_code)
	{
	case dbg_process_start:
		msg("trace:process start\n");
		// reset instruction counter
		break;

		// A step occured (one instruction was executed). This event
		// notification is only generated if step tracing is enabled.
	case dbg_trace:
	{
		/*Create the triton instance for the Instruction*/

		thid_t tid = va_arg(va, thid_t);
		ea_t pc = va_arg(va, ea_t);

		msg("[%d] tracing over: "HEX_FORMAT"\n", g_nb_insn, pc);

		tritonize(pc, tid);

		
		if (g_nb_insn == g_max_insn)
		{
			// stop the trace mode and suspend the process
			disable_step_trace();
			suspend_process();
			msg("process suspended (traced %d instructions)\n", g_max_insn);
		}
		else
		{
			g_nb_insn++;
		}
		break;
	}
	case dbg_bpt:
	{
		//warning("ole!!! un bp");
		break;
	}

	

	case dbg_process_exit:{
		unhook_from_notification_point(HT_DBG, tracer_callback, NULL);
		break;
	}
		
		
	
	break;
	}
	return 0;
}

//---------------------------------------------------------------------------
// Callback for ui notifications
int idaapi ui_callback(void * /*ud*/, int notification_code, va_list va)
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

	break;
	}
	return 0;
}

