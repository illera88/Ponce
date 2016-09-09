//IDA
#include <idp.hpp>
#include <dbg.hpp>
#include <loader.hpp>

//Ponce
#include "globals.hpp"
#include "utils.hpp"
#include "callbacks.hpp"
#include "formChoser.hpp"

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
	"ah_taint_register", // The action name. This acts like an ID and must be unique
	"Taint Register", //The action text.
	&ah_taint_register, //The action handler.
	"Ctrl + R", //Optional: the action shortcut
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
	"ah_symbolize_register", // The action name. This acts like an ID and must be unique
	"Symbolize Register", //The action text.
	&ah_symbolize_register, //The action handler.
	"Ctrl + R", //Optional: the action shortcut
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
	"ah_taint_memory", // The action name. This acts like an ID and must be unique
	"Taint Memory", //The action text.
	&ah_taint_memory, //The action handler.
	"Ctrl + M", //Optional: the action shortcut
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
	"ah_symbolize_memory", // The action name. This acts like an ID and must be unique
	"Symbolize Memory", //The action text.
	&ah_symbolize_memory, //The action handler.
	"Ctrl + S", //Optional: the action shortcut
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
			for (unsigned int i = myPathConstraints.size() - 1; i >= 0; i--)
			{
				auto path_constraint = myPathConstraints[i];
				if (path_constraint.conditionAddr == pc)
				{
					std::vector <triton::ast::AbstractNode *> expr;
					//First we add to the expresion all the previous path constrains
					unsigned int j;
					for (j = 0; j < i; j++)
					{
						if (cmdOptions.showExtraDebugInfo)
							msg("Keeping condition %d\n", j);
						triton::__uint ripId = myPathConstraints[j].conditionRipId;
						auto symExpr = triton::api.getFullAstFromId(ripId);
						triton::__uint takenAddr = myPathConstraints[j].takenAddr;
						expr.push_back(triton::ast::assert_(triton::ast::equal(symExpr, triton::ast::bv(takenAddr, symExpr->getBitvectorSize()))));
					}
					if (cmdOptions.showExtraDebugInfo)
						msg("Inverting condition %d\n", i);
					//And now we negate the selected condition
					triton::__uint ripId = myPathConstraints[i].conditionRipId;
					auto symExpr = triton::api.getFullAstFromId(ripId);
					triton::__uint notTakenAddr = myPathConstraints[i].notTakenAddr;
					if (cmdOptions.showExtraDebugInfo)
						msg("ripId: %d notTakenAddr: "HEX_FORMAT"\n", ripId, notTakenAddr);
					expr.push_back(triton::ast::assert_(triton::ast::equal(symExpr, triton::ast::bv(notTakenAddr, symExpr->getBitvectorSize()))));
					//Time to solve
					auto final_expr = triton::ast::compound(expr);
					if (cmdOptions.showDebugInfo)
						msg("[+] Solving formula...\n");
					std::stringstream ss;
					ss << final_expr;
					if (cmdOptions.showExtraDebugInfo)
						msg("Formula: %s\n", ss.str().c_str());
					auto model = triton::api.getModel(final_expr);
					if (model.size() > 0)
					{
						msg("Solution found! Values:\n");
						for (auto it = model.begin(); it != model.end(); it++)
						{
							auto symbVar = triton::api.getSymbolicVariableFromId(it->first);
							std::string  symbVarComment = symbVar->getComment();
							triton::engines::symbolic::symkind_e symbVarKind = symbVar->getKind();
							triton::uint512 secondValue = it->second.getValue();
							/*if (symbVarKind == triton::engines::symbolic::symkind_e::MEM)
								//newinput->memOperand.push_back(triton::arch::MemoryAccess(symbVar->getKindValue(), symbVar->getSize() / 8, secondValue));
							else if (symbVarKind == triton::engines::symbolic::symkind_e::REG)
								//newinput->regOperand.push_back(triton::arch::Register(symbVar->getKindValue(), secondValue));*/
							//We represent the number different 
							switch (symbVar->getSize())
							{
								case 8:
									msg(" - %s (%s):%#02x (%c)\n", it->second.getName().c_str(), symbVarComment.c_str(), secondValue.convert_to<uchar>(), secondValue.convert_to<uchar>());
									break;
								case 16:
									msg(" - %s (%s):%#04x\n", it->second.getName().c_str(), symbVarComment.c_str(), secondValue.convert_to<ushort>());
									break;
								case 32:
									msg(" - %s (%s):%#08x\n", it->second.getName().c_str(), symbVarComment.c_str(), secondValue.convert_to<uint>());
									break;
								case 64:
									msg(" - %s (%s):%#16llx\n", it->second.getName().c_str(), symbVarComment.c_str(), secondValue.convert_to<uint64>());
									break;
								default:
									msg("Unsupported size for the symbolic variable: %s (%s)\n", it->second.getName().c_str(), symbVarComment.c_str());
							}
						}
					}
					else
						msg("No solution found :(\n");
					break;
				}
			}
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
	"ah_solve", // The action name. This acts like an ID and must be unique
	"Solve formula", //The action text.
	&ah_solve, //The action handler.
	"Ctrl + S", //Optional: the action shortcut
	"Solve a selected constraint", //Optional: the action tooltip (available in menus/toolbar)
	201); //Optional: the action icon (shows when in menus/toolbars)

struct ah_negate_t : public action_handler_t
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
			for (unsigned int i = myPathConstraints.size() - 1; i >= 0; i--)
			{
				auto path_constraint = myPathConstraints[i];
				if (path_constraint.conditionAddr == pc)
				{
					std::vector <triton::ast::AbstractNode *> expr;
					//First we add to the expresion all the previous path constrains
					unsigned int j;
					for (j = 0; j < i; j++)
					{
						if (cmdOptions.showExtraDebugInfo)
							msg("Keeping condition %d\n", j);
						triton::__uint ripId = myPathConstraints[j].conditionRipId;
						auto symExpr = triton::api.getFullAstFromId(ripId);
						triton::__uint takenAddr = myPathConstraints[j].takenAddr;
						expr.push_back(triton::ast::assert_(triton::ast::equal(symExpr, triton::ast::bv(takenAddr, symExpr->getBitvectorSize()))));
					}
					if (cmdOptions.showExtraDebugInfo)
						msg("Inverting condition %d\n", i);
					//And now we negate the selected condition
					triton::__uint ripId = myPathConstraints[i].conditionRipId;
					auto symExpr = triton::api.getFullAstFromId(ripId);
					triton::__uint notTakenAddr = myPathConstraints[i].notTakenAddr;
					if (cmdOptions.showExtraDebugInfo)
						msg("ripId: %d notTakenAddr: "HEX_FORMAT"\n", ripId, notTakenAddr);
					expr.push_back(triton::ast::assert_(triton::ast::equal(symExpr, triton::ast::bv(notTakenAddr, symExpr->getBitvectorSize()))));
					//Time to solve
					auto final_expr = triton::ast::compound(expr);
					if (cmdOptions.showDebugInfo)
						msg("[+] Solving formula...\n");
					std::stringstream ss;
					ss << final_expr;
					if (cmdOptions.showExtraDebugInfo)
						msg("Formula: %s\n", ss.str().c_str());
					auto model = triton::api.getModel(final_expr);
					if (model.size() > 0)
					{
						msg("Solution found! Values:\n");
						for (auto it = model.begin(); it != model.end(); it++)
						{
							auto symbVar = triton::api.getSymbolicVariableFromId(it->first);
							std::string  symbVarComment = symbVar->getComment();
							triton::engines::symbolic::symkind_e symbVarKind = symbVar->getKind();
							triton::uint512 secondValue = it->second.getValue();
							/*if (symbVarKind == triton::engines::symbolic::symkind_e::MEM)
							//newinput->memOperand.push_back(triton::arch::MemoryAccess(symbVar->getKindValue(), symbVar->getSize() / 8, secondValue));
							else if (symbVarKind == triton::engines::symbolic::symkind_e::REG)
							//newinput->regOperand.push_back(triton::arch::Register(symbVar->getKindValue(), secondValue));*/
							//We represent the number different 
							switch (symbVar->getSize())
							{
							case 8:
								msg(" - %s (%s):%#02x (%c)\n", it->second.getName().c_str(), symbVarComment.c_str(), secondValue.convert_to<uchar>(), secondValue.convert_to<uchar>());
								break;
							case 16:
								msg(" - %s (%s):%#04x\n", it->second.getName().c_str(), symbVarComment.c_str(), secondValue.convert_to<ushort>());
								break;
							case 32:
								msg(" - %s (%s):%#08x\n", it->second.getName().c_str(), symbVarComment.c_str(), secondValue.convert_to<uint>());
								break;
							case 64:
								msg(" - %s (%s):%#16llx\n", it->second.getName().c_str(), symbVarComment.c_str(), secondValue.convert_to<uint64>());
								break;
							default:
								msg("Unsupported size for the symbolic variable: %s (%s)\n", it->second.getName().c_str(), symbVarComment.c_str());
							}
							//We need to inject the solution in the memory/registers
							if (symbVarKind == triton::engines::symbolic::symkind_e::MEM)
							{
								auto address = symbVar->getKindValue();
								put_many_bytes((ea_t)address, &secondValue, symbVar->getSize() / 8);
							}
							else if (symbVarKind == triton::engines::symbolic::symkind_e::REG)
							{
								auto reg = symbVar->getKindValue();
								triton::arch::Register r = triton::arch::Register((uint32)reg, 0);
								set_reg_val(r.getName().c_str(), (uint64)secondValue);
							}
						}
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
					}
					else
						msg("No solution found :(\n");
					break;
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
			//Only enabled with symbolize conditions
			//If we are in runtime and it is the last instruction we can test if it is symbolize
			if (last_triton_instruction != NULL && last_triton_instruction->getAddress() == action_update_ctx_t->cur_ea && last_triton_instruction->isBranch() && last_triton_instruction->isSymbolized())
				return AST_ENABLE;
		}
		return AST_DISABLE;
	}
};
static ah_negate_t ah_negate;

static const action_desc_t action_IDA_negate = ACTION_DESC_LITERAL(
	"ah_negate", // The action name. This acts like an ID and must be unique
	"Negate condition", //The action text.
	&ah_negate, //The action handler.
	"Ctrl + N", //Optional: the action shortcut
	"Negate the current condition", //Optional: the action tooltip (available in menus/toolbar)
	201); //Optional: the action icon (shows when in menus/toolbars)


struct createSnapshot_ah_t : public action_handler_t
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
static createSnapshot_ah_t createSnapshot_ah;

static const action_desc_t action_IDA_createSnapshot = ACTION_DESC_LITERAL(
	"Snapshot",
	"Create Snapshot", 
	&createSnapshot_ah,
	"Ctrl-S", 
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
	"Ctrl-O", //Optional: the action shortcut
	"Show the Ponce configuration", //Optional: the action tooltip (available in menus/toolbar)
	201); //Optional: the action icon (shows when in menus/toolbars)

/*This list defined all the actions for the plugin*/
struct action action_list[] =
{
	{ &action_IDA_taint_register, { BWN_DISASM, BWN_CPUREGS, NULL }, true, false, "Taint/"},
	{ &action_IDA_taint_memory, { BWN_DISASM, BWN_DUMP, NULL }, true, false, "Taint/" },
	{ &action_IDA_symbolize_register, { BWN_DISASM, BWN_CPUREGS, NULL }, false, true, "Symbolic/"},
	{ &action_IDA_symbolize_memory, { BWN_DISASM, BWN_DUMP, NULL }, false, true, "Symbolic/" },
	{ &action_IDA_solve, { BWN_DISASM, NULL }, false, true, "SMT/" },
	{ &action_IDA_negate, { BWN_DISASM, NULL }, false, true, "SMT/" },
	{ &action_IDA_createSnapshot, { BWN_DISASM, NULL }, true, true, "Snapshot/"},
	{ NULL, NULL, NULL }
};