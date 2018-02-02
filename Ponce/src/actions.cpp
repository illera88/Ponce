//! \file
/*
**  Copyright (c) 2016 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

//IDA
#include <idp.hpp>
#include <dbg.hpp>
#include <loader.hpp>
#include <kernwin.hpp>

//Ponce
#include "globals.hpp"
#include "utils.hpp"
#include "callbacks.hpp"
#include "formConfiguration.hpp"
#include "formTaintSymbolizeInput.hpp"
#include "actions.hpp"
#include "formTaintWindow.hpp"

//Triton
#include "api.hpp"
#include "x86Specifications.hpp"

struct ah_taint_register_t : public action_handler_t
{
	/*Event called when the user taint a register*/
	virtual int idaapi activate(action_activation_ctx_t *)
	{
		// Get the address range selected, or return false if there was no selection
		bool res = false;
#ifdef __IDA70__
		qstring selected;
		uint32 flags;
		res = get_highlight(&selected, get_current_viewer(), &flags);
#else
		char selected[20];
		res = get_highlighted_identifier(selected, 20, 0);
#endif

		if (res)
		{
			triton::arch::Register register_to_taint;
#ifdef __IDA70__
			res = str_to_register(std::string(selected.c_str()), register_to_taint);
#else
			res = str_to_register(std::string(selected), register_to_taint);
#endif
			if (res)
			{
				msg("[!] Tainting register %s\n", selected);
				register_to_taint.setConcreteValue(triton::api.getConcreteRegisterValue(register_to_taint, true));
				triton::api.taintRegister(register_to_taint);
				//When the user taints something for the first time we should enable step_tracing
				start_tainting_or_symbolic_analysis();
				//If last_instruction is not set this instruction is not analyze
				if (ponce_runtime_status.last_triton_instruction == NULL)
				{
					reanalize_current_instruction();
					return 0;
				}
				//If the register tainted is a source for the instruction then we need to reanalize the instruction
				//So the self instruction will be tainted
				auto read_registers = ponce_runtime_status.last_triton_instruction->getReadRegisters();
				for (auto it = read_registers.begin(); it != read_registers.end(); it++)
				{
					auto reg = it->first;
					//msg("Register read: %s\n", reg.getName().c_str());
					if (reg.getId() == register_to_taint.getId())
					{
						reanalize_current_instruction();
						break;
					}
				}
			}
		}
		return 0;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK)
		{
#ifdef __IDA70__
			qstring selected;
			uint32 flags;
			if (get_highlight(&selected, get_current_viewer(), &flags))
			{
				triton::arch::Register register_to_taint;
				if (str_to_register(std::string(selected.c_str()), register_to_taint))
					return AST_ENABLE;
			}
#else
			char selected[20];
			if (get_highlighted_identifier(selected, 20, 0))
			{
				triton::arch::Register register_to_taint;
				if (str_to_register(std::string(selected), register_to_taint))
					return AST_ENABLE;
			}
#endif
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
	50); //Optional: the action icon (shows when in menus/toolbars)

struct ah_symbolize_register_t : public action_handler_t
{
	/*Event called when the user symbolize a register*/
	virtual int idaapi activate(action_activation_ctx_t *action_activation_ctx)
	{
		bool res = false;
		// Get the address range selected, or return false if there was no selection
#ifdef __IDA70__
		qstring selected;
		uint32 flags;
		res = get_highlight(&selected, get_current_viewer(), &flags);
#else
		char selected[20];
		res = get_highlighted_identifier(selected, 20, 0);
#endif
		if (res)
		{
			triton::arch::Register register_to_symbolize;
#ifdef __IDA70__
			res = str_to_register(std::string(selected.c_str()), register_to_symbolize);
#else
			res = str_to_register(std::string(selected), register_to_symbolize);
#endif
			if (res)
			{
				msg("[!] Symbolizing register %s\n", selected);
				char comment[256];
				qsnprintf(comment, 256, "Reg %s at address: " HEX_FORMAT "", selected, action_activation_ctx->cur_ea);
				register_to_symbolize.setConcreteValue(triton::api.getConcreteRegisterValue(register_to_symbolize, true));
				triton::api.convertRegisterToSymbolicVariable(register_to_symbolize, std::string(comment));

				/*When the user symbolize something for the first time we should enable step_tracing*/
				start_tainting_or_symbolic_analysis();
				//If last_instruction is not set this instruction is not analyze
				if (ponce_runtime_status.last_triton_instruction == NULL)
				{
					reanalize_current_instruction();
					return 0;
				}
				//If the register symbolize is a source for the instruction then we need to reanalize the instruction
				//So the self instruction will be tainted
				auto read_registers = ponce_runtime_status.last_triton_instruction->getReadRegisters();
				for (auto it = read_registers.begin(); it != read_registers.end(); it++)
				{
					auto reg = it->first;
					if (reg.getId() == register_to_symbolize.getId())
					{
						reanalize_current_instruction();
						break;
					}
				}
			}
		}
		return 0;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx_t)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK)
		{
#ifdef __IDA70__
			qstring selected;
			uint32 flags;
			if (get_highlight(&selected, get_current_viewer(), &flags))
			{
				triton::arch::Register register_to_symbolize;
				if (str_to_register(std::string(selected.c_str()), register_to_symbolize))
					return AST_ENABLE;
			}
#else
			char selected[20];
			if (get_highlighted_identifier(selected, 20, 0))
			{
				triton::arch::Register register_to_symbolize;
				if (str_to_register(std::string(selected), register_to_symbolize))
					return AST_ENABLE;
			}
#endif
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
	50); //Optional: the action icon (shows when in menus/toolbars)

struct ah_taint_memory_t : public action_handler_t
{
	/*Event called when the user taint a memory*/
	virtual int idaapi activate(action_activation_ctx_t *action_activation_ctx)
	{
		ea_t selection_starts = 0;
		ea_t selection_ends = 0;
		//If we are in the hex view windows we use the selected bytes
#ifdef __IDA70__
		if (action_activation_ctx->widget_type == BWN_DUMP)
#else
		if (action_activation_ctx->form_type == BWN_DUMP)
#endif
		{
//This menu is only enable in the HEX DUMP view in IDA 6.9x
#ifdef __IDA68__
			return 0;
#else
			if (action_activation_ctx->cur_sel.from.at == NULL || action_activation_ctx->cur_sel.to.at == NULL)
			{
				return 0;
			}
			//We get the selection bounds from the action activation context
			selection_starts = action_activation_ctx->cur_sel.from.at->toea();
			selection_ends = action_activation_ctx->cur_sel.to.at->toea();
#endif
		}
		//In the dissas windows we use the whole item selected. If we have a string we can't select only some bytes from the dissas windows
#ifdef __IDA70__
		else if (action_activation_ctx->widget_type == BWN_DISASM)
#else
		else if (action_activation_ctx->form_type == BWN_DISASM)
#endif
		{
			//We don't know how to do this :( 
			//msg("[!] Disassembly windows no supported for memory tainting...yet\n");
			//So we ask to the user for the memory and the size
			if (!prompt_window_taint_symbolize(0, &selection_starts, &selection_ends))
				return 0;
		}
		else
			return 0;
		//The selection ends in the last item, we need to add 1 to calculate the length
		ea_t selection_length = selection_ends - selection_starts + 1;
		msg("[+] Tainting memory from " HEX_FORMAT " to " HEX_FORMAT ". Total: %d bytes\n", selection_starts, selection_ends, (int)selection_length);
		//Tainting all the selected memory
		taint_all_memory(selection_starts, selection_length);
		/*When the user taints something for the first time we should enable step_tracing*/
		start_tainting_or_symbolic_analysis();
		//If last_instruction is not set this instruction is not analyze
		if (ponce_runtime_status.last_triton_instruction == NULL)
		{
			reanalize_current_instruction();
			return 0;
		}
		//We reanalyse the instruction where the pc is right now
		auto store_access_list = ponce_runtime_status.last_triton_instruction->getStoreAccess();
		for (auto it = store_access_list.begin(); it != store_access_list.end(); it++)
		{
			triton::arch::MemoryAccess memory_access = it->first;
			//If the address is inside the range just tainted, then reanalize
			if (memory_access.getAddress() >= selection_starts && memory_access.getAddress() < selection_starts + selection_length)
			{
				reanalize_current_instruction();
				break;
			}
		}
		return 0;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx_t)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK)
		{
#ifdef __IDA70__
			if (action_update_ctx_t->widget_type == BWN_DUMP)
#else
			if (action_update_ctx_t->form_type == BWN_DUMP)
#endif
			{
//This menu is only enable in the HEX DUMP view in IDA 6.9x
#ifdef __IDA68__
				return AST_DISABLE;
#else
				if (action_update_ctx_t->cur_sel.from.at != NULL && action_update_ctx_t->cur_sel.to.at != NULL)
				{
					auto selection_starts = action_update_ctx_t->cur_sel.from.at->toea();
					auto selection_ends = action_update_ctx_t->cur_sel.to.at->toea();
					int diff = (int)(selection_ends - selection_starts);
					if (diff >= 0)
						return AST_ENABLE;
				}
#endif
			}
			else
			{
				return AST_ENABLE;
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
	50); //Optional: the action icon (shows when in menus/toolbars)

struct ah_symbolize_memory_t : public action_handler_t
{
	/*Event called when the user symbolize a memory*/
	virtual int idaapi activate(action_activation_ctx_t *action_activation_ctx)
	{
		ea_t selection_starts = 0;
		ea_t selection_ends = 0;
		//If we are in the hex view windows we use the selected bytes
#ifdef __IDA70__
		if (action_activation_ctx->widget_type == BWN_DUMP)
#else
		if (action_activation_ctx->form_type == BWN_DUMP)
#endif
		{
			//This menu is only enable in the HEX DUMP view in IDA 6.9x
#ifdef __IDA68__
			return 0;
#else
			if (action_activation_ctx->cur_sel.from.at == NULL || action_activation_ctx->cur_sel.to.at == NULL)
				return 0;
			//We get the selection bounds from the action activation context
			selection_starts = action_activation_ctx->cur_sel.from.at->toea();
			selection_ends = action_activation_ctx->cur_sel.to.at->toea();
#endif
		}
		//In the dissas windows we use the whole item selected. If we have a string we can't select only some bytes from the dissas windows
#ifdef __IDA70__
		else if (action_activation_ctx->widget_type == BWN_DISASM)
#else
		else if (action_activation_ctx->form_type == BWN_DISASM)
#endif
		{
			//We ask to the user for the memory and the size
			if (!prompt_window_taint_symbolize(0, &selection_starts, &selection_ends))
				return 0;
		}
		else
			return 0;

		//The selection ends in the last item which is included, so we need to add 1 to calculate the length
		auto selection_length = selection_ends - selection_starts + 1;
		msg("[+] Symbolizing memory from " HEX_FORMAT " to " HEX_FORMAT ". Total: %d bytes\n", selection_starts, selection_ends, (int)selection_length);
		//Tainting all the selected memory
		char comment[256];
		qsnprintf(comment, 256, "Mem " HEX_FORMAT "-" HEX_FORMAT " at address: " HEX_FORMAT "", selection_starts, selection_starts + selection_length, action_activation_ctx->cur_ea);
		symbolize_all_memory(selection_starts, selection_length, comment);
		/*When the user taints something for the first time we should enable step_tracing*/
		start_tainting_or_symbolic_analysis();
		//If last_instruction is not set this instruction is not analyze
		if (ponce_runtime_status.last_triton_instruction == NULL)
		{
			reanalize_current_instruction();
			return 0;
		}
		//We reanalyse the instruction where the pc is right now if the instruction was reading the memory that was just symbolized
		auto load_access_list = ponce_runtime_status.last_triton_instruction->getLoadAccess();
		for (auto it = load_access_list.begin(); it != load_access_list.end(); it++)
		{
			triton::arch::MemoryAccess memory_access = it->first;
			//If the address is inside the range just symbolized, then reanalize
			if (memory_access.getAddress() >= selection_starts && memory_access.getAddress() < selection_starts + selection_length)
			{
				reanalize_current_instruction();
				break;
			}
		}
		return 0;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx_t)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK)
		{
#ifdef __IDA70__
			if (action_update_ctx_t->widget_type == BWN_DUMP)
#else
			if (action_update_ctx_t->form_type == BWN_DUMP)
#endif
			{
				//This menu is only enable in the HEX DUMP view in IDA 6.9x
#ifdef __IDA68__
				return AST_DISABLE;
#else
				if (action_update_ctx_t->cur_sel.from.at != NULL && action_update_ctx_t->cur_sel.to.at != NULL)
				{
					auto selection_starts = action_update_ctx_t->cur_sel.from.at->toea();
					auto selection_ends = action_update_ctx_t->cur_sel.to.at->toea();
					int diff = (int)(selection_ends - selection_starts);
					if (diff >= 0)
						return AST_ENABLE;
				}
#endif
			}
			else
			{
				return AST_ENABLE;
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
	50); //Optional: the action icon (shows when in menus/toolbars)

struct ah_negate_and_inject_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *action_activation_ctx)
	{
		//This is only working from the disassembly windows
#ifdef __IDA70__
		if (action_activation_ctx->widget_type == BWN_DISASM)
#else
		if (action_activation_ctx->form_type == BWN_DISASM)
#endif
		{
			ea_t pc = action_activation_ctx->cur_ea;
			msg("[+] Negating condition at " HEX_FORMAT "\n", pc);
			//We want to negate the last path contraint at the current address, so we use as a bound the size of the path constrains
			unsigned int bound = ponce_runtime_status.myPathConstraints.size() - 1;
			auto input_ptr = solve_formula(pc, bound);
			if (input_ptr != NULL)
			{
				//We need to modify the last path constrain
				auto temp = ponce_runtime_status.myPathConstraints[bound].notTakenAddr;
				ponce_runtime_status.myPathConstraints[bound].notTakenAddr = ponce_runtime_status.myPathConstraints[bound].takenAddr;
				ponce_runtime_status.myPathConstraints[bound].takenAddr = temp;
				//We need to modify the condition flags to negate the condition
				if (ponce_runtime_status.last_triton_instruction->getAddress() == pc)
				{
					negate_flag_condition(ponce_runtime_status.last_triton_instruction);
				}
				// We set the results obtained from solve_formula
				set_SMT_results(input_ptr);

				//delete it after setting the proper results
				delete input_ptr;
			}
		}
		return 0;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx_t)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK)
		{
			//If we are in runtime and it is the last instruction we test if it is symbolize
			if (ponce_runtime_status.last_triton_instruction != NULL && ponce_runtime_status.last_triton_instruction->getAddress() == action_update_ctx_t->cur_ea && ponce_runtime_status.last_triton_instruction->isBranch() && ponce_runtime_status.last_triton_instruction->isSymbolized())
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
	58); //Optional: the action icon (shows when in menus/toolbars)

struct ah_negate_inject_and_restore_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *action_activation_ctx)
	{
		//This is only working from the disassembly windows
#ifdef __IDA70__
		if (action_activation_ctx->widget_type == BWN_DISASM)
#else
		if (action_activation_ctx->form_type == BWN_DISASM)
#endif
		{
			ea_t pc = action_activation_ctx->cur_ea;
			msg("[+] Negating condition at " HEX_FORMAT "\n", pc);
			//We need to get the instruction associated with this address, we look for the addres in the map
			//We want to negate the last path contraint at the current address, so we traverse the myPathconstraints in reverse

			unsigned int bound = ponce_runtime_status.myPathConstraints.size() - 1;
			auto input_ptr = solve_formula(pc, bound);
			if (input_ptr != NULL)
			{
				//Restore the snapshot
				snapshot.restoreSnapshot();

				// We set the results obtained from solve_formula
				set_SMT_results(input_ptr);

				//delete it after setting the proper results
				delete input_ptr;
			}
		}
		return 0;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *action_update_ctx_t)
	{
		//Only if process is being debugged
		if (get_process_state() != DSTATE_NOTASK && snapshot.exists())
		{
			//If we are in runtime and it is the last instruction we test if it is symbolize
			if (ponce_runtime_status.last_triton_instruction != NULL && ponce_runtime_status.last_triton_instruction->getAddress() == action_update_ctx_t->cur_ea && ponce_runtime_status.last_triton_instruction->isBranch() && ponce_runtime_status.last_triton_instruction->isSymbolized())
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
	145); //Optional: the action icon (shows when in menus/toolbars)

struct ah_create_snapshot_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		//This is the address where the popup was shown, what we need is the xip
		//ea_t pc = ctx->cur_ea;
		uint64 xip;
		get_reg_val(TRITON_REG_XIP.getName().c_str(), &xip);
		set_cmt((ea_t)xip, "Snapshot taken here", false);
		snapshot.takeSnapshot();
		snapshot.setAddress((ea_t)xip); // We will use this address later to delete the comment
		msg("Snapshot Taken\n");
		return 0;
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
	129);

struct ah_restore_snapshot_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		snapshot.restoreSnapshot();
		msg("Snapshot restored\n");
		return 0;
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
	"Ctrl+Shift+S",
	NULL,
	128);

struct ah_delete_snapshot_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		snapshot.resetEngine();
		msg("[+] Snapshot removed\n");
		return 0;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		//Only if there is an existent shapshot
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
	130);

struct ah_show_config_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		prompt_conf_window();
		return 0;
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
	156); //Optional: the action icon (shows when in menus/toolbars)

struct ah_show_taintWindow_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		//So we don't reopen twice the same window
#ifdef __IDA70__
		auto form = find_widget("Taint Window");
#else
		auto form = find_tform("Taint Window");
#endif
		if (form != NULL){
			//let's update it and change to it
			fill_entryList();
#ifdef __IDA70__
			activate_widget(form, true);
#else
			switchto_tform(form, true);
#endif
		}
		
		else
			create_taint_window();
	
		return 0;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		return AST_ENABLE_ALWAYS;
	}
};
static ah_show_taintWindow_t ah_show_taintWindow;

action_desc_t action_IDA_show_taintWindow = ACTION_DESC_LITERAL(
	"Ponce:show_taintWindows", // The action name. This acts like an ID and must be unique
	"Show Taint/Symbolic items", //The action text.
	&ah_show_taintWindow, //The action handler.
	"Alt+Shift+T", //Optional: the action shortcut
	"Show all the taint or symbolic items", //Optional: the action tooltip (available in menus/toolbar)
	157); //Optional: the action icon (shows when in menus/toolbars)

struct ah_unload_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		term();
		return 0;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		return AST_ENABLE_ALWAYS;
	}
};
static ah_unload_t ah_unload;

action_desc_t action_IDA_unload = ACTION_DESC_LITERAL(
	"Ponce:unload", // The action name. This acts like an ID and must be unique
	"Unload plugin", //The action text.
	&ah_unload, //The action handler.
	"Ctrl+Shift+U", //Optional: the action shortcut
	"Unload the plugin", //Optional: the action tooltip (available in menus/toolbar)
	138); //Optional: the action icon (shows when in menus/toolbars)

struct ah_execute_native_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		if (ask_for_execute_native())
		{
			//Deleting previous snapshot
			snapshot.resetEngine();
			//Disabling step tracing...
			disable_step_trace();
			//And continue! (F9)
			continue_process();
		}
		return 0;
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
	113); //Optional: the action icon (shows when in menus/toolbars)

struct ah_enable_disable_tracing_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		if (ponce_runtime_status.runtimeTrigger.getState())
		{
			if (ask_for_execute_native())
			{
				//Deleting previous snapshot
				snapshot.resetEngine();
				//Disabling step tracing...
				disable_step_trace();
				ponce_runtime_status.runtimeTrigger.disable();
				if (cmdOptions.showDebugInfo)
					msg("Disabling step tracing\n");
			}
		}
		else
		{
			//Enabling step tracing...
			enable_step_trace(true);
			set_step_trace_options(0);
			ponce_runtime_status.tracing_start_time = 0;
			//Enabling the trigger
			ponce_runtime_status.analyzed_thread = get_current_thread();
			ponce_runtime_status.runtimeTrigger.enable();
			//And analyzing current instruction
			reanalize_current_instruction();
			if (cmdOptions.showDebugInfo)
				msg("Enabling step tracing\n");
		}
		return 0;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		//We are using this event to change the text of the action
		if (ponce_runtime_status.runtimeTrigger.getState())
		{
			update_action_label(ctx->action, "Disable ponce tracing");
			update_action_icon(ctx->action, 62);
		}
		else
		{
			update_action_label(ctx->action, "Enable ponce tracing");
			update_action_icon(ctx->action, 61);
		}
		
		//So...we use this update event to update the ea, and then in the 
		//ui_finish_populating_tform_popup to add the sunmenus for solve
		popup_menu_ea = ctx->cur_ea;
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
	61); //Optional: the action icon (shows when in menus/toolbars)


struct ah_solve_formula_sub_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		//We extract the solved index from the action name
		unsigned int condition_index = atoi(ctx->action);
		if (cmdOptions.showDebugInfo)
			msg("[+] Solving condition at %d\n", condition_index);
		auto input_ptr = solve_formula(ctx->cur_ea, condition_index);
		if (input_ptr != NULL)
			delete input_ptr;
		return 0;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		return AST_ENABLE;
	}
};
ah_solve_formula_sub_t ah_solve_formula_sub;

action_desc_t action_IDA_solve_formula_sub = ACTION_DESC_LITERAL(
	"Ponce:solve_formula_sub", // The action name. This acts like an ID and must be unique
	"", //The action text.
	&ah_solve_formula_sub, //The action handler.
	"", //Optional: the action shortcut
	"The solves a specific conditions and shows the result in the output window", //Optional: the action tooltip (available in menus/toolbar)
	13); //Optional: the action icon (shows when in menus/toolbars)

/*This list defined all the actions for the plugin*/
struct action action_list[] =
{
	{ &action_IDA_enable_disable_tracing, { BWN_DISASM, __END__ }, true, true, "" },
	
	{ &action_IDA_taint_register, { BWN_DISASM, BWN_CPUREGS, __END__ }, true, false, "Taint/"},
	{ &action_IDA_taint_memory, { BWN_DISASM, BWN_DUMP, __END__ }, true, false, "Taint/" },

	{ &action_IDA_symbolize_register, { BWN_DISASM, BWN_CPUREGS, __END__ }, false, true, "Symbolic/"},
	{ &action_IDA_symbolize_memory, { BWN_DISASM, BWN_DUMP, __END__ }, false, true, "Symbolic/" },

	{ &action_IDA_negate, { BWN_DISASM, __END__ }, false, true, "SMT/" },
	{ &action_IDA_negateInjectRestore, { BWN_DISASM, __END__ }, true, true, "SMT/" },

	{ &action_IDA_createSnapshot, { BWN_DISASM, __END__ }, true, true, "Snapshot/"},
	{ &action_IDA_restoreSnapshot, { BWN_DISASM, __END__ }, true, true, "Snapshot/" },
	{ &action_IDA_deleteSnapshot, { BWN_DISASM, __END__ }, true, true, "Snapshot/" },
	{ &action_IDA_execute_native, { BWN_DISASM, __END__ }, true, true, "" },
	{ NULL, __END__, __END__ }
};
