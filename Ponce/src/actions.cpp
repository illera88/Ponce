//IDA
#include <idp.hpp>
#include <dbg.hpp>
#include <loader.hpp>
#include <kernwin.hpp>

//Ponce
#include "globals.hpp"
#include "utils.hpp"
#include "callbacks.hpp"
#include "formChoser.hpp"
#include "actions.hpp"

//Triton
#include "api.hpp"

struct ah_taint_register_t : public action_handler_t
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
				start_tainting_or_symbolic_analysis();
				//If the register tainted is a source for the instruction then we need to reanalize the instruction
				//So the self instruction will be tainted
				auto read_registers = last_triton_instruction->getReadRegisters();
				for (auto it = read_registers.begin(); it != read_registers.end(); it++)
				{
					auto reg = it->first;
					//msg("Register read: %s\n", reg.getName().c_str());
					if (reg.getId() == register_to_taint->getId())
					{
						//msg("reanalyzing\n");
						reanalize_current_instruction();
					}
				}
			}
		}
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK)
		{
			char selected[20];
			if (get_highlighted_identifier(selected, 20, 0))
			{
				triton::arch::Register *register_to_taint = str_to_register(std::string(selected));
				if (register_to_taint != NULL)
					return AST_ENABLE;
			}
		}
		return AST_DISABLE;
	}
};
static ah_taint_register_t ah_taint_register;

static const action_desc_t action_IDA_taint_register = ACTION_DESC_LITERAL(
	"Ponce:taint_register", // The action name. This acts like an ID and must be unique
	"Taint Register", //The action text.
	&ah_taint_register, //The action handler.
	"Ctrl+Shift+R", //Optional: the action shortcut
	"Taint the selected register", //Optional: the action tooltip (available in menus/toolbar)
	199); //Optional: the action icon (shows when in menus/toolbars)

struct ah_symbolize_register_t : public action_handler_t
{
	/*Event called when the user symbolize a register*/
	virtual int idaapi activate(action_activation_ctx_t *action_activation_ctx)
	{
		// Get the address range selected, or return false if there was no selection
		char selected[20];
		if (get_highlighted_identifier(selected, 20, 0))
		{
			triton::arch::Register *register_to_symbolize = str_to_register(std::string(selected));
			if (register_to_symbolize != NULL)
			{
				msg("[!] Symbolizing register %s\n", selected);
				char comment[256];
				sprintf_s(comment, 256, "Reg %s at address: "HEX_FORMAT"\n", selected, action_activation_ctx->cur_ea);
				triton::api.convertRegisterToSymbolicVariable(*register_to_symbolize, comment);
				/*When the user symbolize something for the first time we should enable step_tracing*/
				start_tainting_or_symbolic_analysis();
				//If the register symbolize is a source for the instruction then we need to reanalize the instruction
				//So the self instruction will be tainted
				auto read_registers = last_triton_instruction->getReadRegisters();
				for (auto it = read_registers.begin(); it != read_registers.end(); it++)
				{
					auto reg = it->first;
					//msg("Register read: %s\n", reg.getName().c_str());
					if (reg.getId() == register_to_symbolize->getId())
					{
						//msg("reanalyzing\n");
						reanalize_current_instruction();
					}
				}
			}
		}
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx_t)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK)
		{
			char selected[20];
			if (get_highlighted_identifier(selected, 20, 0))
			{
				triton::arch::Register *register_to_taint = str_to_register(std::string(selected));
				if (register_to_taint != NULL)
					return AST_ENABLE;
			}
		}
		return AST_DISABLE;
	}
};
static ah_symbolize_register_t ah_symbolize_register;

static const action_desc_t action_IDA_symbolize_register = ACTION_DESC_LITERAL(
	"Ponce:symbolize_register", // The action name. This acts like an ID and must be unique
	"Symbolize Register", //The action text.
	&ah_symbolize_register, //The action handler.
	"Ctrl+Shift+R", //Optional: the action shortcut
	"Symbolize the selected register", //Optional: the action tooltip (available in menus/toolbar)
	199); //Optional: the action icon (shows when in menus/toolbars)

struct ah_taint_memory_t : public action_handler_t
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
		if (cmdOptions.showDebugInfo)
			msg("[+] Tainting memory from "HEX_FORMAT" to "HEX_FORMAT". Total: %d bytes\n", selection_starts, selection_ends, selection_length);
		//Tainting all the selected memory
		taint_all_memory(selection_starts, selection_length);
		/*When the user taints something for the first time we should enable step_tracing*/
		start_tainting_or_symbolic_analysis();
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

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx_t)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK)
		{
			if (action_update_ctx_t->form_type == BWN_DUMP)
			{
				if (action_update_ctx_t->cur_sel.from.at != NULL && action_update_ctx_t->cur_sel.to.at != NULL)
				{
					auto selection_starts = action_update_ctx_t->cur_sel.from.at->toea();
					auto selection_ends = action_update_ctx_t->cur_sel.to.at->toea();
					if (selection_ends - selection_starts > 0)
						return AST_ENABLE;
				}
			}
		}
		return AST_DISABLE;
	}
};
static ah_taint_memory_t ah_taint_memory;

static const action_desc_t action_IDA_taint_memory = ACTION_DESC_LITERAL(
	"Ponce:taint_memory", // The action name. This acts like an ID and must be unique
	"Taint Memory", //The action text.
	&ah_taint_memory, //The action handler.
	"Ctrl+Shift+M", //Optional: the action shortcut
	"Taint the selected register", //Optional: the action tooltip (available in menus/toolbar)
	200); //Optional: the action icon (shows when in menus/toolbars)

struct ah_symbolize_memory_t : public action_handler_t
{
	/*Event called when the user symbolize a memory*/
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
		if (cmdOptions.showDebugInfo)
			msg("[+] Symbolizing memory from "HEX_FORMAT" to "HEX_FORMAT". Total: %d bytes\n", selection_starts, selection_ends, selection_length);
		//Tainting all the selected memory
		char comment[256];
		sprintf_s(comment, 256, "Mem "HEX_FORMAT"-"HEX_FORMAT" at address: "HEX_FORMAT"\n", selection_starts, selection_starts + selection_length, action_activation_ctx->cur_ea);
		symbolize_all_memory(selection_starts, selection_length, comment);
		/*When the user taints something for the first time we should enable step_tracing*/
		start_tainting_or_symbolic_analysis();
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

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx_t)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK)
		{
			if (action_update_ctx_t->form_type == BWN_DUMP)
			{
				if (action_update_ctx_t->cur_sel.from.at != NULL && action_update_ctx_t->cur_sel.to.at != NULL)
				{
					auto selection_starts = action_update_ctx_t->cur_sel.from.at->toea();
					auto selection_ends = action_update_ctx_t->cur_sel.to.at->toea();
					if (selection_ends - selection_starts > 0)
						return AST_ENABLE;
				}
			}
		}
		return AST_DISABLE;
	}
};
static ah_symbolize_memory_t ah_symbolize_memory;

static const action_desc_t action_IDA_symbolize_memory = ACTION_DESC_LITERAL(
	"Ponce:symbolize_memory", // The action name. This acts like an ID and must be unique
	"Symbolize Memory", //The action text.
	&ah_symbolize_memory, //The action handler.
	"Ctrl+Shift+M", //Optional: the action shortcut
	"Symbolize the selected register", //Optional: the action tooltip (available in menus/toolbar)
	200); //Optional: the action icon (shows when in menus/toolbars)


struct ah_solve_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *action_activation_ctx)
	{
		//This is only working from the disassembly windows
		if (action_activation_ctx->form_type == BWN_DISASM)
		{
			ea_t pc = action_activation_ctx->cur_ea;
			if (cmdOptions.showDebugInfo)
				msg("[+] Solving condition at "HEX_FORMAT"\n", pc);
			//We need to get the instruction associated with this address, we look for the addres in the map
			//We want to negate the last path contraint at the current address, so we traverse the myPathconstraints in reverse	
			auto input_ptr = solve_formula(pc, NULL);
			if (input_ptr != NULL)
				delete input_ptr;
		}
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx_t)
	{
		//Only enabled with symbolize conditions
		//If we are in runtime and it is the last instruction we can test if it is symbolize
		if (last_triton_instruction != NULL && last_triton_instruction->getAddress() == action_update_ctx_t->cur_ea && last_triton_instruction->isBranch() && last_triton_instruction->isSymbolized())
			return AST_ENABLE;
		//If we are in offline mode we can check if the condition is in the path constrains
		for (unsigned int i = 0; i < myPathConstraints.size(); i++)
		{
			if (myPathConstraints[i].conditionAddr == action_update_ctx_t->cur_ea)
				return AST_ENABLE;
		}
		return AST_DISABLE;
	}
};
static ah_solve_t ah_solve;

static const action_desc_t action_IDA_solve = ACTION_DESC_LITERAL(
	"Ponce:solve", // The action name. This acts like an ID and must be unique
	"Solve formula", //The action text.
	&ah_solve, //The action handler.
	"Ctrl+Shift+S", //Optional: the action shortcut
	"Solve a selected constraint", //Optional: the action tooltip (available in menus/toolbar)
	201); //Optional: the action icon (shows when in menus/toolbars)

struct ah_negate_and_inject_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *action_activation_ctx)
	{
		//This is only working from the disassembly windows
		if (action_activation_ctx->form_type == BWN_DISASM)
		{
			ea_t pc = action_activation_ctx->cur_ea;
			if (cmdOptions.showDebugInfo)
				msg("[+] Negating condition at "HEX_FORMAT"\n", pc);
			//We need to get the instruction associated with this address, we look for the addres in the map
			//We want to negate the last path contraint at the current address, so we traverse the myPathconstraints in reverse

			auto input_ptr = solve_formula(pc, NULL);
			if (input_ptr != NULL)
			{
				//We need to modify the last path constrain
				auto temp = myPathConstraints[myPathConstraints.size() - 1].notTakenAddr;
				myPathConstraints[myPathConstraints.size() - 1].notTakenAddr = myPathConstraints[myPathConstraints.size() - 1].takenAddr;
				myPathConstraints[myPathConstraints.size() - 1].takenAddr = temp;
				//We need to modify the condition flags to negate the condition
				if (last_triton_instruction->getAddress() == pc)
				{
					negate_flag_condition(last_triton_instruction);
					/*auto regs = last_triton_instruction->getReadRegisters();
					for (auto it = regs.begin(); it != regs.end(); it++)
					{
						auto reg = it->first;
						//If the register is a flag and it is symbolized, we have a candidate to negate
						if (reg.isFlag() && triton::api.getSymbolicRegisterId(reg) != triton::engines::symbolic::UNSET && triton::api.getSymbolicExpressionFromId(triton::api.getSymbolicRegisterId(reg))->isSymbolized())
						{
							uint64 val;
							auto old_value = get_reg_val(reg.getName().c_str(), &val);
							//Negating flag
							val = !val;
							set_reg_val(reg.getName().c_str(), val);
							break;
						}
					}*/
				}
				// We set the results obtained from solve_formula
				set_SMT_results(input_ptr);

				//delete it after setting the proper results
				delete input_ptr;
			}
		}
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx_t)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK)
		{
			//Only enabled with symbolize conditions
			//If we are in runtime and it is the last instruction we can test if it is symbolize
			if (last_triton_instruction != NULL && last_triton_instruction->getAddress() == action_update_ctx_t->cur_ea && last_triton_instruction->isBranch() && last_triton_instruction->isSymbolized())
				return AST_ENABLE;
		}
		return AST_DISABLE;
	}
};
static ah_negate_and_inject_t ah_negate_and_inject;

static const action_desc_t action_IDA_negate = ACTION_DESC_LITERAL(
	"Ponce:negate_and_inject", // The action name. This acts like an ID and must be unique
	"Negate & Inject", //The action text.
	&ah_negate_and_inject, //The action handler.
	"Ctrl+Shift+N", //Optional: the action shortcut
	"Negate the current condition and inject the solution into memory", //Optional: the action tooltip (available in menus/toolbar)
	201); //Optional: the action icon (shows when in menus/toolbars)

struct ah_negate_inject_and_restore_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *action_activation_ctx)
	{
		//This is only working from the disassembly windows
		if (action_activation_ctx->form_type == BWN_DISASM)
		{
			ea_t pc = action_activation_ctx->cur_ea;
			if (cmdOptions.showDebugInfo)
				msg("[+] Negating condition at "HEX_FORMAT"\n", pc);
			//We need to get the instruction associated with this address, we look for the addres in the map
			//We want to negate the last path contraint at the current address, so we traverse the myPathconstraints in reverse

			auto input_ptr = solve_formula(pc, NULL);
			if (input_ptr != NULL)
			{
				//We need to modify the last path constrain
				auto temp = myPathConstraints[myPathConstraints.size() - 1].notTakenAddr;
				myPathConstraints[myPathConstraints.size() - 1].notTakenAddr = myPathConstraints[myPathConstraints.size() - 1].takenAddr;
				myPathConstraints[myPathConstraints.size() - 1].takenAddr = temp;
				//We need to modify some of the symbolized flag to negate the condition
				if (last_triton_instruction->getAddress() == pc)
				{
					auto regs = last_triton_instruction->getReadRegisters();
					for (auto it = regs.begin(); it != regs.end(); it++)
					{
						auto reg = it->first;
						//If the register is a flag and it is symbolized, we have a candidate to negate
						if (reg.isFlag() && triton::api.getSymbolicRegisterId(reg) != triton::engines::symbolic::UNSET && triton::api.getSymbolicExpressionFromId(triton::api.getSymbolicRegisterId(reg))->isSymbolized())
						{
							uint64 val;
							auto old_value = get_reg_val(reg.getName().c_str(), &val);
							//Negating flag
							val = !val;
							set_reg_val(reg.getName().c_str(), val);
							break;
						}
					}
				}
				// We set the results obtained from solve_formula
				set_SMT_results(input_ptr);

				//delete it after setting the proper results
				delete input_ptr;

				//Restore the snapshot
				snapshot.restoreSnapshot();
			}
		}
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx_t)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK && snapshot.exists())
		{
			//Only enabled with symbolize conditions
			//If we are in runtime and it is the last instruction we can test if it is symbolize
			if (last_triton_instruction != NULL && last_triton_instruction->getAddress() == action_update_ctx_t->cur_ea && last_triton_instruction->isBranch() && last_triton_instruction->isSymbolized())
				return AST_ENABLE;
		}
		return AST_DISABLE;
	}
};
static ah_negate_inject_and_restore_t ah_negate_inject_and_restore;

static const action_desc_t action_IDA_negateInjectRestore = ACTION_DESC_LITERAL(
	"Ponce:negate_inject_restore", // The action name. This acts like an ID and must be unique
	"Negate, Inject & Restore snapshot", //The action text.
	&ah_negate_inject_and_restore, //The action handler.
	"Ctrl+Shift+I", //Optional: the action shortcut
	"Negates a condition, inject the solution and restore the snapshot", //Optional: the action tooltip (available in menus/toolbar)
	164); //Optional: the action icon (shows when in menus/toolbars)

struct ah_create_snapshot_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		snapshot.takeSnapshot();
		msg("Snapshot Taken\n");
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		//Only if process is being debugged and there is not previous snaphot
		if (get_process_state() != DSTATE_NOTASK && !snapshot.exists())
			return AST_ENABLE;
		else
			return AST_DISABLE;
	}
};
static ah_create_snapshot_t ah_create_snapshot;

static const action_desc_t action_IDA_createSnapshot = ACTION_DESC_LITERAL(
	"Ponce:create_snapshot",
	"Create Execution Snapshot", 
	&ah_create_snapshot,
	"Ctrl+Shift+C", 
	NULL, 
	15);

struct ah_restore_snapshot_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		snapshot.restoreSnapshot();
		msg("Snapshot restored\n");
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		//Only if process is being debugged and there is an existent shapshot
		if (get_process_state() != DSTATE_NOTASK && snapshot.exists())
			return AST_ENABLE;
		else
			return AST_DISABLE;
	}
};
static ah_restore_snapshot_t ah_restore_snapshot;

static const action_desc_t action_IDA_restoreSnapshot = ACTION_DESC_LITERAL(
	"Ponce:restore_snapshot",
	"Restore Execution Snapshot",
	&ah_restore_snapshot,
	"Ctrl+Shift+R",
	NULL,
	15);

struct ah_delete_snapshot_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		snapshot.resetEngine();
		msg("Snapshot removed\n");
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		//Only if process is being debugged and there is an existent shapshot
		if (snapshot.exists())
			return AST_ENABLE;
		else
			return AST_DISABLE;
	}
};
static ah_delete_snapshot_t ah_delete_snapshot;

static const action_desc_t action_IDA_deleteSnapshot = ACTION_DESC_LITERAL(
	"Ponce:delete_snapshot",
	"Delete Execution Snapshot",
	&ah_delete_snapshot,
	"Ctrl+Shift+D",
	NULL,
	15);

struct ah_show_config_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		prompt_conf_window();
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		return AST_ENABLE_ALWAYS;
	}
};
static ah_show_config_t ah_show_config;

action_desc_t action_IDA_show_config = ACTION_DESC_LITERAL(
	"Ponce:show_config", // The action name. This acts like an ID and must be unique
	"Show config", //The action text.
	&ah_show_config, //The action handler.
	"Ctrl+Shift+P", //Optional: the action shortcut
	"Show the Ponce configuration", //Optional: the action tooltip (available in menus/toolbar)
	186); //Optional: the action icon (shows when in menus/toolbars)

struct ah_execute_native_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		if (ask_for_execute_native())
		{
			//Disabling step tracing...
			enable_step_trace(false);
			//And continue! (F9)
			continue_process();
		}
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		return AST_ENABLE_ALWAYS;
	}
};
static ah_execute_native_t ah_execute_native;

action_desc_t action_IDA_execute_native = ACTION_DESC_LITERAL(
	"Ponce:execute_native", // The action name. This acts like an ID and must be unique
	"Execute native", //The action text.
	&ah_execute_native, //The action handler.
	"Ctrl+Shift+F9", //Optional: the action shortcut
	"Execute native without tracing every instruction until next breakpoint", //Optional: the action tooltip (available in menus/toolbar)
	186); //Optional: the action icon (shows when in menus/toolbars)

struct ah_enable_disable_tracing_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		if (runtimeTrigger.getState())
		{
			if (ask_for_execute_native())
			{
				//Disabling step tracing...
				disable_step_trace();
				runtimeTrigger.disable();
				if (cmdOptions.showDebugInfo)
					msg("Disabling step tracing\n");
			}
		}
		else
		{
			//Enabling step tracing...
			enable_step_trace(true);
			//Enabling the trigger
			runtimeTrigger.enable();
			//And analyzing current instruction
			reanalize_current_instruction();
			if (cmdOptions.showDebugInfo)
				msg("Enabling step tracing\n");
		}
		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		//We are using this event to change the text of the action
		if (runtimeTrigger.getState())
			update_action_label(ctx->action, "Disable ponce tracing");
		else
			update_action_label(ctx->action, "Enable ponce tracing");
		
		return AST_ENABLE;
	}
};
static ah_enable_disable_tracing_t ah_enable_disable_tracing;

//We need to define this struct before the action handler because we are using it inside the handler
action_desc_t action_IDA_enable_disable_tracing = ACTION_DESC_LITERAL(
	"Ponce:enable_disable_tracing",
	"Enable/Disable ponce tracing", //The action text.
	&ah_enable_disable_tracing, //The action handler.
	"Ctrl+Shift+E", //Optional: the action shortcut
	"Enable or Disable the ponce tracing", //Optional: the action tooltip (available in menus/toolbar)
	188); //Optional: the action icon (shows when in menus/toolbars)

/*This list defined all the actions for the plugin*/
struct action action_list[] =
{
	{ &action_IDA_enable_disable_tracing, { BWN_DISASM, NULL }, true, true, "" },
	{ &action_IDA_taint_register, { BWN_DISASM, BWN_CPUREGS, NULL }, true, false, "Taint/"},
	{ &action_IDA_taint_memory, { BWN_DISASM, BWN_DUMP, NULL }, true, false, "Taint/" },

	{ &action_IDA_symbolize_register, { BWN_DISASM, BWN_CPUREGS, NULL }, false, true, "Symbolic/"},
	{ &action_IDA_symbolize_memory, { BWN_DISASM, BWN_DUMP, NULL }, false, true, "Symbolic/" },

	{ &action_IDA_solve, { BWN_DISASM, NULL }, false, true, "SMT/" },
	{ &action_IDA_negate, { BWN_DISASM, NULL }, false, true, "SMT/" },
	{ &action_IDA_negateInjectRestore, { BWN_DISASM, NULL }, true, true, "SMT/" },

	{ &action_IDA_createSnapshot, { BWN_DISASM, NULL }, true, true, "Snapshot/"},
	{ &action_IDA_restoreSnapshot, { BWN_DISASM, NULL }, true, true, "Snapshot/" },
	{ &action_IDA_deleteSnapshot, { BWN_DISASM, NULL }, true, true, "Snapshot/" },
	{ &action_IDA_execute_native, { BWN_DISASM, NULL }, true, true, "" },
	{ NULL, NULL, NULL }
};