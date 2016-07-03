#include <idp.hpp>
#include <dbg.hpp>
#include <loader.hpp>

#include "globals.hpp"


struct printsel_TRegister : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *)
	{
		msg(" EA: %p register type=%d", get_screen_ea(), get_opnum());
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
	"TRegister", // The action name. This acts like an ID and must be unique
	"Taint Register", //The action text.
	&taint_register, //The action handler.
	"Ctrl + H", //Optional: the action shortcut
	"Taint the selected register", //Optional: the action tooltip (available in menus/toolbar)
	199); //Optional: the action icon (shows when in menus/toolbars)

struct printsel_TMemory : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *)
	{
		msg(" EA: %p register type=%d", get_screen_ea(), get_opnum());
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
	"TMemory", // The action name. This acts like an ID and must be unique
	"Taint Memory", //The action text.
	&taint_memory, //The action handler.
	"Ctrl + H", //Optional: the action shortcut
	"Taint the selected register", //Optional: the action tooltip (available in menus/toolbar)
	200); //Optional: the action icon (shows when in menus/toolbars)

struct action action_list[] =
{
	{ "TRegister", "Taint Register", &action_IDA_taint_register, { BWN_DISASM, BWN_IMPORTS, NULL } },
	{ "TMemory", "Taint Memory", &action_IDA_taint_memory, { BWN_DISASM, NULL } },
	{ NULL, NULL, NULL }
};