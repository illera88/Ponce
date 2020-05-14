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
        //Iterate over tainted memory
        for (const auto& addr : api.getTaintedMemory()) {
            item_t* list_entry = new item_t();

            list_entry->address = addr;
            list_entry->isTainted_or_symbolized = true;
            list_entry->value = api.getConcreteMemoryValue(addr, false);

            global_list->push_back(list_entry);
        }

        //Iterate over tainted registers
        for (const auto& reg : api.getTaintedRegisters()) {
            item_t* list_entry = new item_t();

            list_entry->register_name = reg->getName();
            list_entry->isTainted_or_symbolized = true;
            list_entry->value = api.getConcreteRegisterValue(*reg, false);

            global_list->push_back(list_entry);
        }
    }
    else if (cmdOptions.use_symbolic_engine) {
        //Iterate over symbolic memory
        for (const auto& [Addr, SymExpr] : api.getSymbolicMemory()) {
            item_t* list_entry = new item_t();
            
            list_entry->isTainted_or_symbolized = SymExpr->isSymbolized();
            list_entry->id = SymExpr->getId();
            list_entry->address = Addr;
            for (const auto& [SymVarId, SymVar] : api.getSymbolicVariables()) {
                if (SymVar->getOrigin() == Addr) {
                    auto SymVar_name = SymVar->getName(); // this should be SymVar_0 ...
                    break;
                }
            }
            list_entry->comment = SymExpr->getComment();
            list_entry->value = api.getConcreteMemoryValue(SymExpr->getOriginMemory(), false);

            global_list->push_back(list_entry);
        }

        for (const auto& [reg_id, SymExpr] : api.getSymbolicRegisters()) {
            item_t* list_entry = new item_t();
            auto reg = api.getRegister(reg_id);
 
            list_entry->isTainted_or_symbolized = SymExpr->isSymbolized();
            list_entry->id = SymExpr->getId();
            list_entry->register_name = reg.getName();
            list_entry->comment = SymExpr->getComment();
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
    : chooser_t(CH_CAN_REFRESH, qnumber(widths_), widths_, header_, cmdOptions.use_tainting_engine ? "Ponce Taint Items" : "Ponce Symbolic Items") {
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
    if(!li->register_name.empty())
        cols[2].sprnt("%s", li->register_name.c_str());
    cols[3].sprnt(MEM_FORMAT, li->value.convert_to<ea_t>()); // ToDo: this should not be converted
    cols[4].sprnt("%s", li->isTainted_or_symbolized ? "x" : "");
    msg("comment %s", li->comment.c_str());
    //if(!li->comment.empty())
    //    cols[6].sprnt("%s", li->comment.c_str());

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

