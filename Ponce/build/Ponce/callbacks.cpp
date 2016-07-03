


#include <idp.hpp>
#include <dbg.hpp>
#include <loader.hpp>

#include "globals.hpp"
#include "callbacks.hpp"


int idaapi tracer_callback(void * /*user_data*/, int notification_code, va_list va)
{
	/*if (g_nb_insn == 0)
	myfunc();*/

	switch (notification_code)
	{
	case dbg_process_start:
		msg("trace:process start\n");
		// reset instruction counter
		break;

		//case dbg_run_to:
		//	msg("tracer: entrypoint reached\n");
		//	enable_insn_trace(true);
		//	// while continue_process() would work here too, request+run is more universal
		//	// because they do not ignore the request queue
		//	request_continue_process();
		//	run_requests();
		//	break;

		// A step occured (one instruction was executed). This event
		// notification is only generated if step tracing is enabled.
	case dbg_trace:
	{
		/*thid_t tid =*/ va_arg(va, thid_t);
		ea_t ip = va_arg(va, ea_t);
		msg("[%d] tracing over: %a\n", g_nb_insn, ip);
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
	}
	case dbg_bpt:
	{
		warning("ole!!! un bp");
	}

	break;

	case dbg_process_exit:
		unhook_from_notification_point(HT_DBG, tracer_callback, NULL);
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
		for (int i = 0;; i++){
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



		//if (get_tform_type(form) == BWN_DISASM)
		//{
		//	TPopupMenu *popup_handle = va_arg(va, TPopupMenu *);
		//	TCustomControl *view = get_tform_idaview(form);
		//	if (view != NULL)
		//	{
		//		for (int i = 0;;i++){
		//			if (action_list[i].name == NULL)
		//				break;
		//			attach_action_to_popup(form, popup_handle, action_list[i].name, NULL, SETMENU_FIRST);
		//		}


		//		/*attach_action_to_popup(form, popup_handle, ACTION_NAME_T_REGISTER);
		//		attach_action_to_popup(form, popup_handle, ACTION_NAME_T_MEMORY);*/
		//		attach_action_to_popup(form, popup_handle, ACTION_NAME_T_REGISTER, NULL, SETMENU_FIRST);
		//		attach_action_to_popup(form, popup_handle, ACTION_NAME_T_MEMORY, NULL, SETMENU_FIRST);
		//	}
		//}
	}
	case dbg_process_exit:{
		unhook_from_notification_point(HT_DBG, ui_callback, NULL);
		break;
	}

	break;
	}
	return 0;
}

