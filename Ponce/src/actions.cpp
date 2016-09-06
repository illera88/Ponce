//IDA
#include <idp.hpp>
#include <dbg.hpp>
#include <loader.hpp>

//Ponce
#include "globals.hpp"
#include "utils.hpp"
#include "callbacks.hpp"

//Triton
#include "api.hpp"

struct printsel_TRegister : public action_handler_t
{
	/*Event called when the user taint a register*/
	virtual int idaapi activate(action_activation_ctx_t *)
	{
		// Get the address range selected, or return false if there was no selection
		char selected[20];
		if (get_highlighted_identifier(selected, 20, 0))
		{
			triton::arch::Register *register_to_taint = str_to_register(std::string(selected));
			if (register_to_taint != NULL)
			{
				msg("[!] Tainted register %s\n", selected);
				triton::api.taintRegister(*register_to_taint);
				/*When the user taints something for the first time we should enable step_tracing*/
				start_tainting_analysis();
				//If the register tainted is a source for the instruction then we need to reanalize the instruction
				//So the self instruction will be tainted
				auto read_registers = last_triton_instruction->getReadRegisters();
				for (auto it = read_registers.begin(); it != read_registers.end(); it++)
				{
					auto reg = it->first;
					msg("Register read: %s\n", reg.getName().c_str());
					if (reg.getId() == register_to_taint->getId())
					{
						msg("reanalyzing\n");
						reanalize_current_instruction();
					}
				}
			}
		}
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
	"Ctrl + R", //Optional: the action shortcut
	"Taint the selected register", //Optional: the action tooltip (available in menus/toolbar)
	199); //Optional: the action icon (shows when in menus/toolbars)

struct printsel_TMemory : public action_handler_t
{
	/*Event called when the user taint a memory*/
	virtual int idaapi activate(action_activation_ctx_t *action_activation_ctx)
	{
		ea_t selection_starts = 0;
		ea_t selection_ends = 0;
		//If we are in the hex view windows we use the selected bytes
		if (action_activation_ctx->form_type == BWN_DUMP)
		{
			if (action_activation_ctx->cur_sel.from.at == NULL || action_activation_ctx->cur_sel.to.at == NULL)
			{
				//msg("from or to is NULL\n");
				return 0;
			}
			//We get the selection bounds from the action activation context
			selection_starts = action_activation_ctx->cur_sel.from.at->toea();
			selection_ends = action_activation_ctx->cur_sel.to.at->toea();
		}
		//In the dissas windows we use the whole item selected. If we have a string we can't select only some bytes from the dissas windows
		else if (action_activation_ctx->form_type == BWN_DISASM)
		{
			msg("[!] Disassembly windows no supported for memory tainting...yet\n");
			/*int selected = read_selection(&selection_starts, &selection_ends);
			msg("selectted : %d\n", selected);
			if (!selected)*/
			return 0;

		}
		else
			return 0;

		//The selection ends in the last item, we need to add 1 to calculate the length
		unsigned int selection_length = selection_ends - selection_starts + 1;
		if (DEBUG)
			msg("[+] Tainting memory from "HEX_FORMAT" to "HEX_FORMAT". Total: %d bytes\n", selection_starts, selection_ends, selection_length);
		//Tainting all the selected memory
		taint_all_memory(selection_starts, selection_length);
		/*When the user taints something for the first time we should enable step_tracing*/
		start_tainting_analysis();
		//We reanalyse the instruction where the pc is right now
		//Todo: We could only do this if the instruction is affected by the memory modified
		reanalize_current_instruction();
//		ea_t saddr, eaddr;

		// Get the address range selected, or return false if
		// there was no selection
		//ToDo: This function is working well with saddr, but not with eaddr. 
		//If we are selecting 2 bytes in the hexview and the item has 8 bytes, the
		//eaddrr always point to the end of the item (an instruction or variable)
		//int selected = read_selection(&saddr, &eaddr);
		//char selected2[20];

		//if (get_highlighted_identifier(selected2, 20, 0))
		//	msg("Selected_output_text %s\n", selected2);
		//if (selected)
		//	msg("Selected range: %a -> %a\n", saddr, eaddr);
		//else
		//	msg("No selection.\n");
		


		//msg("p0.x: %d ,p1.x: %d\n", p0.x, p1.x);
		//msg("Type: %s\n", typeid(p0.at).name());

		//is_something_tainted = true;
		//runtimeTrigger.enable();
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
	"Ctrl + M", //Optional: the action shortcut
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
	"Ctrl + S", //Optional: the action shortcut
	"Solve a selected constraint", //Optional: the action tooltip (available in menus/toolbar)
	201); //Optional: the action icon (shows when in menus/toolbars)


struct formchooser_ah_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		msg("Menu item clicked. Current selection:");
		for (int i = 0, n = ctx->chooser_selection.size(); i < n; ++i)
			msg(" %d", ctx->chooser_selection[i]);
		msg("\n");
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		bool ok = ctx->form_type == BWN_CHOOSER;
		if (ok)
		{
			char name[MAXSTR];
			ok = get_tform_title(ctx->form, name, sizeof(name))
				&& strneq(name, "Form with choosers", qstrlen("Form with choosers"));
		}
		return ok ? AST_ENABLE_FOR_FORM : AST_DISABLE_FOR_FORM;
	}
};
static formchooser_ah_t formchooser_ah;

static const action_desc_t action_IDA_choser = ACTION_DESC_LITERAL(
	"Choser",
	"User Choser", 
	&formchooser_ah, 
	"Ctrl-K", 
	NULL, 
	12);




/*This list defined all the actions for the plugin*/
struct action action_list[] =
{
	{ "TRegister", "Taint Register", &action_IDA_taint_register, { BWN_DISASM, BWN_CPUREGS, NULL } },
	{ "TMemory", "Taint Memory", &action_IDA_taint_memory, { BWN_DISASM, BWN_DUMP, NULL } },
	{ "Solver", "Solve formula", &action_IDA_solver, { BWN_DISASM, NULL } },
	//{ "Choser", "User Choser", &action_IDA_choser, { BWN_DISASM, NULL } },
	{ NULL, NULL, NULL }
};