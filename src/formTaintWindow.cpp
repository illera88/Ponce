//! \file
/*
**  Copyright (c) 2016 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

//C++
#include <string>
#include <sstream>

//Triton
#include <triton/api.hpp>

//IDA
#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <bytes.hpp>
#include <kernwin.hpp>

//Ponce
#include "formTaintWindow.hpp"
#include "globals.hpp"

const char* popup_menu_names[] = {
    "Insert",					// not active
    "Delete",					// not active
    "Dump segment to disk",		// menu entry for dumping the segment
    "Refresh"					// not active
};

entrylist_t* global_list;


void fill_entryList() {

    //We clear the list
    global_list->clear();

    if (cmdOptions.use_tainting_engine) {
        auto taintedMemoryList = api.getTaintedMemory();
        auto taintedRegistersList = api.getTaintedRegisters();

        //Iterate over tainted memory
        for (auto iterator = taintedMemoryList.begin(); iterator != taintedMemoryList.end(); ++iterator) {
            item_t* list_entry = new item_t();

            list_entry->address = *iterator;
            list_entry->isTainted_or_symbolized = true;
            list_entry->value = api.getConcreteMemoryValue(*iterator);

            global_list->push_back(list_entry);
        }

        //Iterate over tainted registers
        for (auto iterator = taintedRegistersList.begin(); iterator != taintedRegistersList.end(); ++iterator) {
            item_t* list_entry = new item_t();
            auto reg = *(*iterator);

            list_entry->register_name = reg.getName();
            list_entry->isTainted_or_symbolized = true;
            list_entry->value = api.getConcreteRegisterValue(reg, false);

            global_list->push_back(list_entry);
        }
    }
    else if (cmdOptions.use_symbolic_engine) {
        auto symMemMap = api.getSymbolicMemory();
        auto symRegMap = api.getSymbolicRegisters();
        auto ctx = api.getAstContext();
        
        //for (const auto& [SymVarId, SymVar] : api.getSymbolicVariables()) {
        //    if (SymVar->getType() == triton::engines::symbolic::variable_e::MEMORY_VARIABLE) {
        //        auto mem = triton::arch::MemoryAccess(SymVar->getOrigin(), SymVar->getSize() / 8);
        //        newinput.memOperand.push_back(mem);
        //        api.setConcreteMemoryValue(mem, model_value); // Why
        //    }
        //    else if (SymVar->getType() == triton::engines::symbolic::variable_e::REGISTER_VARIABLE) {
        //        (triton::arch::register_e)SymVar->getOrigin()
        //        auto reg = triton::arch::Register(*api.getCpuInstance(), );
        //        newinput.regOperand.push_back(reg);
        //        api.setConcreteRegisterValue(reg, model_value); // Why?
        //        //ToDo: add concretizeRegister()??
        //    }
        //}

        //Iterate over symbolic memory
        for (auto iterator = symMemMap.begin(); iterator != symMemMap.end(); iterator++) {
            auto symbExpr = iterator->second;
            item_t* list_entry = new item_t();

            auto variables = triton::ast::search(symbExpr->getAst(), triton::ast::ast_e::VARIABLE_NODE);

            list_entry->isTainted_or_symbolized = symbExpr->isSymbolized();
            list_entry->id = symbExpr->getId();
            list_entry->address = iterator->first;
            list_entry->comment = symbExpr->getComment();
            list_entry->value = api.getConcreteMemoryValue(symbExpr->getOriginMemory(), false);
            //list_entry->value = symbExpr->getOriginMemory().getConcreteValue();

            global_list->push_back(list_entry);
        }

        //Iterate over symbolic registers
        for (auto iterator = symRegMap.begin(); iterator != symRegMap.end(); iterator++) {
            auto symbExpr = iterator->second;
            auto reg = symbExpr->getOriginRegister();
            item_t* list_entry = new item_t();

            list_entry->isTainted_or_symbolized = symbExpr->isSymbolized();
            list_entry->id = symbExpr->getId();
            list_entry->register_name = reg.getName();
            list_entry->comment = symbExpr->getComment();
            list_entry->value = api.getConcreteRegisterValue(reg, false);

            global_list->push_back(list_entry);
        }
    }
}

// column widths
struct entry_chooser_t : public chooser_t
{
protected:
    static const int widths_[];
public:

    // this object must be allocated using `new`
    entry_chooser_t();
    virtual ~entry_chooser_t() {
        if (global_list != NULL) {
            global_list->clear();
            qfree(global_list);
            global_list = NULL;
        }
    }
    // function that returns number of lines in the list
    virtual size_t idaapi get_count() const { return global_list->size(); }

    // function that generates the list line
    virtual void idaapi get_row(qstrvec_t* cols, int* icon_,
        chooser_item_attrs_t* attrs,
        size_t n) const;

    // function that is called when the user wants to refresh the chooser
    virtual cbret_t idaapi refresh(ssize_t n) {
        fill_entryList();
        return adjust_last_item(n);  // try to preserve the cursor
    }

    // function that is called when the user wants to close the chooser
    virtual cbret_t idaapi destroy(ssize_t n) {
        if (global_list != NULL) {
            global_list->clear();
            qfree(global_list);
            global_list = NULL;
        }
        return adjust_last_item(n);  // try to preserve the cursor
    }
};

const int entry_chooser_t::widths_[] = { CHCOL_DEC | 8,
10,
7,
16,
6,
16 };

// column headers
 char* header_[] =
{
    "Id",
    "Address",
    "Reg Name",
    "Value",
    "",
    "Comment",
};

inline entry_chooser_t::entry_chooser_t()
    : chooser_t(CH_CAN_REFRESH, qnumber(widths_), widths_, header_, cmdOptions.use_tainting_engine? "Taint Items" :"Symbolic Items") {
    CASSERT(qnumber(widths_) == qnumber(header_));

    if (global_list == NULL) {
        global_list = new entrylist_t();
    }
    //Fill the list with Triton info
    fill_entryList();
}

// function that generates the list line
void idaapi entry_chooser_t::get_row(qstrvec_t* cols_, int*, chooser_item_attrs_t*,
    size_t n) const {
    qstrvec_t& cols = *cols_;

    item_t* li = global_list->at(n);

    cols[0].sprnt("%d", li->id);
    if (li->address == 0)
        cols[1].sprnt("%s", "");
    else
        cols[1].sprnt(MEM_FORMAT, li->address);
    cols[2].sprnt("%s", li->register_name.c_str());
    cols[3].sprnt(MEM_FORMAT, li->value.convert_to<ea_t>()); // ToDo: this should not be converted
    cols[4].sprnt("%s", li->isTainted_or_symbolized ? "x" : "");
    cols[6].sprnt("%s", li->comment.c_str());

}


void create_taint_window() {
    //First we set the name depending if we are in tainting or symbolic mode
    if (cmdOptions.use_tainting_engine) {
        header_[4] = "Taint";
    }
    else if (cmdOptions.use_symbolic_engine) {
        header_[4] = "Symbolic";
    }
    // open the window
    entry_chooser_t* ch = new entry_chooser_t();
    ch->choose();
}

