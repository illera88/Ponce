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

#include "callbacks.hpp"
#include "actions.hpp"
#include "globals.hpp"

//#include "api.hpp"
//#include <x86Specifications.hpp>
//
//using namespace triton;
//using namespace triton::arch;
//using namespace triton::arch::x86;

//--------------------------------------------------------------------------
int g_nb_insn = 0;
const int g_max_insn = 20;
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
	for (int i = 0;; i++)
	{
		if (action_list[i].name == NULL){
			break;
		}
		if (!register_action(*action_list[i].callback))
		{
			warning("Failed to register %s actions. Exiting Ponce plugin\n",action_list[i].name);
			return PLUGIN_SKIP;
		}	
	}
	return PLUGIN_KEEP;
}

//--------------------------------------------------------------------------
void idaapi term(void)
{
	// just to be safe
	//unhook_from_notification_point(HT_DBG, tracer_callback, NULL);
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