//#include <ida.hpp>
//#include <idp.hpp>
//#include <loader.hpp>
//#include <kernwin.hpp>
//#include <dbg.hpp>
//

//
//
//void other(){
//	// Set the default start address to the user cursur position
//	ea_t eaddr, saddr = get_screen_ea();
//	// Allow the user to specify a start address
//	askaddr(&saddr, "Address to start tracing at");
//	// Set the end address to the end of the current function
//	func_t *func = get_func(saddr);
//	eaddr = func->endEA;
//	// Allow the user to specify an end address
//	askaddr(&eaddr, "Address to end tracing at");
//	// Queue the following
//	// Run to the start address
//	request_run_to(saddr);
//	// Then enable tracing
//	request_enable_insn_trace();
//	// Run to the end address, tracing all stops in between
//	request_run_to(eaddr);
//	// Turn off tracing once we've hit the end address
//	request_disable_insn_trace();
//	// Stop the process once we have what we want
//	request_exit_process();
//	// Run the above queued requests
//	run_requests();
//
//}
//

//
//int idaapi init(void)
//{
//	return PLUGIN_OK;
//}
//
//void idaapi run(int)
//{
//	warning("yoyoyoyoy");
//	triton::api.setArchitecture(triton::arch::ARCH_X86);
//	myfunc();
//
//}
//
//void idaapi term(void)
//{
//	warning("bye bye");
//}
//
//
//
//plugin_t PLUGIN =
//{
//	IDP_INTERFACE_VERSION,
//	PLUGIN_UNL,           // plugin flags
//	init,                 // initialize
//	NULL,                 // terminate. this pointer may be NULL.
//	run,                  // invoke plugin
//	NULL,                 // long comment about the plugin
//	NULL,                 // multiline help about the plugin
//	"yoyoyo",       // the preferred short name of the plugin
//	NULL                  // the preferred hotkey to run the plugin
//};


#include <idp.hpp>
#include <dbg.hpp>
#include <loader.hpp>


#include <kernwin.hpp>


//#include "api.hpp"
//#include <x86Specifications.hpp>
//
//using namespace triton;
//using namespace triton::arch;
//using namespace triton::arch::x86;

//--------------------------------------------------------------------------
static int g_nb_insn=0;
static const int g_max_insn = 20;
bool hooked = false;
//--------------------------------------------------------------------------


//void myfunc(){
//	/* Set the arch */
//	api.setArchitecture(ARCH_X86_64);
//
//	msg("Name        : %s\n", TRITON_X86_REG_AH.getName().c_str());
//	msg("Size byte   : %d\n", TRITON_X86_REG_AH.getSize());
//	msg("Size bit    : %d\n", TRITON_X86_REG_AH.getBitSize());
//	msg("Highed bit  : %d\n", TRITON_X86_REG_AH.getHigh());
//	msg("Lower  bit  : %d\n", TRITON_X86_REG_AH.getLow());
//	msg("Parent      : %s", TRITON_X86_REG_AH.getParent().getName().c_str());
//
//	msg("----------------------------");
//
//	auto reg = api.getAllRegisters();
//	for (auto it = reg.begin(); it != reg.end(); it++) {
//		RegisterOperand r = **it;
//		msg("%s\n", r.getName().c_str());
//	}
//
//}

#define ACTION_NAME_T_REGISTER "TRegister"
#define ACTION_LABEL_T_REGISTER "Taint Register"
#define ACTION_NAME_T_MEMORY "TMemory"
#define ACTION_LABEL_T_MEMORY "Taint Memory"

struct printsel_TRegister : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *)
	{
		msg("Taint register. Here I should call Triton to taint the register\n");
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *)
	{
		return AST_ENABLE_ALWAYS;
	}
};
static printsel_TRegister taint_register;

static const action_desc_t action_IDA_taint_register = ACTION_DESC_LITERAL(
	ACTION_NAME_T_REGISTER, // The action name. This acts like an ID and must be unique
	ACTION_LABEL_T_REGISTER, //The action text.
	&taint_register, //The action handler.
	"Ctrl + H", //Optional: the action shortcut
	"Taint the selected register", //Optional: the action tooltip (available in menus/toolbar)
	199); //Optional: the action icon (shows when in menus/toolbars)

struct printsel_TMemory : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *)
	{
		msg("Taint Memory. Here I should call Triton to taint the memory\n");
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *)
	{
		return AST_ENABLE_ALWAYS;
	}
};
static printsel_TMemory taint_memory;

static const action_desc_t action_IDA_taint_memory = ACTION_DESC_LITERAL(
	ACTION_NAME_T_MEMORY, // The action name. This acts like an ID and must be unique
	ACTION_LABEL_T_MEMORY, //The action text.
	&taint_memory, //The action handler.
	"Ctrl + H", //Optional: the action shortcut
	"Taint the selected register", //Optional: the action tooltip (available in menus/toolbar)
	200); //Optional: the action icon (shows when in menus/toolbars)



static int idaapi callback(void * /*user_data*/, int notification_code, va_list va)
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
		unhook_from_notification_point(HT_DBG, callback, NULL);
		break;
	}
	return 0;
}

//---------------------------------------------------------------------------
// Callback for ui notifications
static int idaapi ui_callback(void * /*ud*/, int notification_code, va_list va)
{
	switch (notification_code)
	{
		// called when IDA is preparing a context menu for a view
		// Here dynamic context-depending user menu items can be added.
	case ui_populating_tform_popup:
	{
		TForm *form = va_arg(va, TForm *);
		if (get_tform_type(form) == BWN_DISASM)
		{		
			TPopupMenu *popup_handle = va_arg(va, TPopupMenu *);
			TCustomControl *view = get_tform_idaview(form);
			if (view != NULL)
			{
				/*attach_action_to_popup(form, popup_handle, ACTION_NAME_T_REGISTER);
				attach_action_to_popup(form, popup_handle, ACTION_NAME_T_MEMORY);*/
				attach_action_to_popup(form, popup_handle, ACTION_NAME_T_REGISTER, NULL, SETMENU_FIRST);
				attach_action_to_popup(form, popup_handle, ACTION_NAME_T_MEMORY, NULL, SETMENU_FIRST);
			}
		}
	}

	break;
	}
	return 0;
}


//--------------------------------------------------------------------------
void idaapi run(int)
{
	if (!hooked){
		if (!hook_to_notification_point(HT_UI, ui_callback, NULL))
		{
			warning("Could not hook ui callback");
			return;
		}
		hooked = true;
	}
}

//--------------------------------------------------------------------------
int idaapi init(void)
{
	if (!register_action(action_IDA_taint_register) ||
		!register_action(action_IDA_taint_memory)
		)
	{
		warning("Failed to register any of the actions. Exiting Ponce plugin");
		return PLUGIN_SKIP;
	}
	warning("action registered!!!");
	return PLUGIN_KEEP;
}

//--------------------------------------------------------------------------
void idaapi term(void)
{
	// just to be safe
	//unhook_from_notification_point(HT_DBG, callback, NULL);
	unhook_from_notification_point(HT_UI, ui_callback, NULL);
	hooked = false;
}

//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN =
{
	IDP_INTERFACE_VERSION,
	0,                    // plugin flags
	init,                 // initialize
	term,                 // terminate. this pointer may be NULL.
	run,                  // invoke plugin
	"Instruction tracer sample", // long comment about the plugin
	"", // multiline help about the plugin
	"Ponce", // the preferred short name of the plugin
	"Alt-Z" // the preferred hotkey to run the plugin
};