//IDA
#include <idp.hpp>
#include <dbg.hpp>
#include <loader.hpp>

//Ponce
#include "globals.hpp"

//Triton
#include "api.hpp"

struct printsel_TRegister : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *)
	{
		char registers[] = {"eax", "ebx", "ecx","edx","edi", "esi","ebp","esp"};
		msg(" EA: %p register type=%d", get_screen_ea(), get_opnum());
		msg("Taint register. Here I should call Triton to taint the register\n");

		ea_t saddr, eaddr;

		// Get the address range selected, or return false if
		// there was no selection
		//int selected1 = read_selection(&saddr, &eaddr);
		char selected[20];

		if (get_highlighted_identifier(selected, 20, 0))
			msg("Selected_output_text %s\n",selected);
		if (selected && triton::arch::)
			msg("Selected range: %a -> %a\n", saddr, eaddr);
		else
			msg("No selection.\n");

		bool isRegister = false;
		for (int i = 0; i < sizeof(registers); i++){
			if (strcmp(selected, registers[i]) == 0)
				isRegister = true;
		}

		/*When the user taints something for the first time we should enable step_tracing*/
		if (!is_something_tainted){
			enable_insn_trace(true);
		}

		is_something_tainted = true;
		runtimeTrigger.enable();
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
		/*When the user taints something for the first time we should enable step_tracing*/
		if (!is_something_tainted){
			enable_insn_trace(true);
		}
		ea_t saddr, eaddr;

		// Get the address range selected, or return false if
		// there was no selection
		int selected = read_selection(&saddr, &eaddr);
		char selected2[20];

		if (get_highlighted_identifier(selected2, 20, 0))
			msg("Selected_output_text %s\n", selected2);
		if (selected)
			msg("Selected range: %a -> %a\n", saddr, eaddr);
		else
			msg("No selection.\n");
		
		TCustomControl *view = get_current_viewer();
		twinpos_t * p0;
		twinpos_t * p1;
		readsel2(view, p0, p1);


		is_something_tainted = true;
		runtimeTrigger.enable();
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
	"Ctrl + J", //Optional: the action shortcut
	"Taint the selected register", //Optional: the action tooltip (available in menus/toolbar)
	200); //Optional: the action icon (shows when in menus/toolbars)

struct printsel_Solver : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *)
	{
		/*We check if the selected instruction is tainted and if it is at the end of a BB*/
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *)
	{
		return AST_ENABLE_ALWAYS;
	}
};
static printsel_Solver solver;

static const action_desc_t action_IDA_solver = ACTION_DESC_LITERAL(
	"Solver", // The action name. This acts like an ID and must be unique
	"Solve formula", //The action text.
	&solver, //The action handler.
	"Ctrl + J", //Optional: the action shortcut
	"Solve a selected constraint", //Optional: the action tooltip (available in menus/toolbar)
	201); //Optional: the action icon (shows when in menus/toolbars)




struct action action_list[] =
{
	{ "TRegister", "Taint Register", &action_IDA_taint_register, { BWN_DISASM, BWN_CPUREGS, NULL } },
	{ "TMemory", "Taint Memory", &action_IDA_taint_memory, { BWN_DISASM, BWN_DUMP, NULL } },
	{ "Solver", "Solve formula", &action_IDA_solver, { BWN_DISASM, NULL } },
	{ NULL, NULL, NULL }
};