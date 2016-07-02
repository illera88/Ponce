#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <kernwin.hpp>

#include "api.hpp"
#include <x86Specifications.hpp>

using namespace triton;
using namespace triton::arch;
using namespace triton::arch::x86;


void myfunc(){
	/* Set the arch */
	api.setArchitecture(ARCH_X86_64);

	msg("Name        : %s\n", TRITON_X86_REG_AH.getName().c_str());
	msg("Size byte   : %d\n", TRITON_X86_REG_AH.getSize());
	msg("Size bit    : %d\n", TRITON_X86_REG_AH.getBitSize());
	msg("Highed bit  : %d\n", TRITON_X86_REG_AH.getHigh());
	msg("Lower  bit  : %d\n", TRITON_X86_REG_AH.getLow());
	msg("Parent      : %s", TRITON_X86_REG_AH.getParent().getName().c_str());

	msg("----------------------------");

	auto reg = api.getAllRegisters();
	for (auto it = reg.begin(); it != reg.end(); it++) {
		RegisterOperand r = **it;
		msg("%s\n", r.getName().c_str());
	}

}

int idaapi init(void)
{
	return PLUGIN_OK;
}

void idaapi run(int)
{
	warning("yoyoyoyoy");
	triton::api.setArchitecture(triton::arch::ARCH_X86);
	myfunc();

}

plugin_t PLUGIN =
{
	IDP_INTERFACE_VERSION,
	PLUGIN_UNL,           // plugin flags
	init,                 // initialize
	NULL,                 // terminate. this pointer may be NULL.
	run,                  // invoke plugin
	NULL,                 // long comment about the plugin
	NULL,                 // multiline help about the plugin
	"yoyoyo",       // the preferred short name of the plugin
	NULL                  // the preferred hotkey to run the plugin
};
