//IDA
#include <idp.hpp>
#include <dbg.hpp>
#include <loader.hpp>
#include <kernwin.hpp>

//Ponce
#include "callbacks.hpp"
#include "actions.hpp"
#include "globals.hpp"
#include "trigger.hpp"
#include "context.hpp"

//Triton
#include <api.hpp>

//#include <x86Specifications.hpp>
//
//using namespace triton;
//using namespace triton::arch;
//using namespace triton::arch::x86;

//--------------------------------------------------------------------------
int g_nb_insn = 0;
const int g_max_insn = 20;
bool hooked = false;
bool is_something_tainted = false;
Trigger runtimeTrigger;

//--------------------------------------------------------------------------

void triton_init()
{
	//We need to set the architecture for Triton
	//ToDo: We should use the IDA api to get the architecture of the binary, x86 o x64 and set the architecture with that info
	triton::api.setArchitecture(triton::arch::ARCH_X86);
	// Memory access callback
	triton::api.addCallback(needConcreteMemoryValue);
	// Register access callback
	triton::api.addCallback(needConcreteRegisterValue);
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
		if (!hook_to_notification_point(HT_DBG, tracer_callback, NULL))
		{
			warning("Could not hook tracer callback");
			return;
		}
		triton_init();
		msg("Plugin running\n");
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
		//Here we register all the actions
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
	unhook_from_notification_point(HT_UI, ui_callback, NULL);
	unhook_from_notification_point(HT_DBG, tracer_callback, NULL);
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