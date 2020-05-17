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
#include "symVarTable.hpp"
#include "globals.hpp"

const char* popup_menu_names[] = {
    "Insert",					// not active
    "Delete",					// not active
    "Dump segment to disk",		// menu entry for dumping the segment
    "Refresh"					// not active
};

void ponce_table_chooser_t::fill_entryList() {
    //We clear the list
    table_item_list.clear();

    for (const auto& [SymVarId, SymVar] : api.getSymbolicVariables()) {
        list_item_t list_entry;

        list_entry.id = SymVarId;
        if (SymVar->getType() == triton::engines::symbolic::variable_e::MEMORY_VARIABLE) {
            list_entry.address = SymVar->getOrigin();
            list_entry.value = api.getConcreteMemoryValue(list_entry.address, false);
        }
        else if (SymVar->getType() == triton::engines::symbolic::variable_e::REGISTER_VARIABLE) {
            list_entry.register_name = api.getRegister((triton::arch::register_e)SymVar->getOrigin()).getName();
            list_entry.value = api.getConcreteRegisterValue(api.getRegister((triton::arch::register_e)SymVar->getOrigin()), false);
        }

        list_entry.var_name = SymVar->getName();
        list_entry.comment = SymVar->getComment();
        table_item_list.push_back(list_entry);
    }

    return;

    if (cmdOptions.use_tainting_engine) {
        //Iterate over tainted memory
        for (const auto& addr : api.getTaintedMemory()) {
            //list_item_t* list_entry = new list_item_t();
            list_item_t list_entry;

            list_entry.address = addr;
            list_entry.value = api.getConcreteMemoryValue(addr, false);

            table_item_list.push_back(list_entry);
        }

        //Iterate over tainted registers
        for (const auto& reg : api.getTaintedRegisters()) {
            //list_item_t* list_entry = new list_item_t();
            list_item_t list_entry;

            list_entry.register_name = reg->getName();
            list_entry.value = api.getConcreteRegisterValue(*reg, false);

            table_item_list.push_back(list_entry);
        }
    }
    else if (cmdOptions.use_symbolic_engine) {
        
    }
}


const int ponce_table_chooser_t::widths_[] = { 
    CHCOL_DEC | 8,
    12,
    10,
    7,
    16,
    16 
};

// column headers
 const char* ponce_table_chooser_t::header_[] =
{
    "SymVarId",
    "Var Name",
    "Address", // Only for memory
    "Reg Name", // Only for registry 
    "Concrete Value",
    "Comment"
};

ponce_table_chooser_t::ponce_table_chooser_t()
    : chooser_multi_t(CH_CAN_REFRESH, qnumber(widths_), widths_, header_, "Ponce Symbolic Variables") {
    CASSERT(qnumber(widths_) == qnumber(header_));

    //Fill the list with Triton info
    fill_entryList();
}

// function that generates the list line
void idaapi ponce_table_chooser_t::get_row(qstrvec_t* cols_, int*, chooser_item_attrs_t*,
    size_t n) const {
    qstrvec_t& cols = *cols_;

    list_item_t li = table_item_list.at(n);

    cols[0].sprnt("%d", li.id);
    cols[1].sprnt("%s", li.var_name.c_str());
    if(!li.register_name.empty()){
        // its a register entry
        cols[3].sprnt("%s", li.register_name.c_str());
    }
    else{    
        // Its a memory entry
        cols[2].sprnt(MEM_FORMAT, li.address);
        //cols[4].sprnt(MEM_FORMAT, li.value.convert_to<ea_t>()); // ToDo: this should not be converted
    }
    
    
    //cols[4].sprnt(MEM_FORMAT, li.value.convert_to<ea_t>()); // Concrete value
    cols[4].sprnt("%s", li.value.str().c_str()); // Concrete value
    msg("comment %s", li.comment.c_str());
    if(!li.comment.empty())
        cols[5].sprnt("%s", li.comment.c_str());

}



/// Selection changed
/// \note This callback is not supported in the txt-version.
/// \param sel  new selected items
void idaapi ponce_table_chooser_t::select(const sizevec_t& sel) const
{
    //void* thisPtr = (void*)this; //serious hack to get around the const
    //((Ponce_table_chooser_t*)thisPtr)->sv = (sizevec_t*)sel;
    //*sv = sel;
   // sv = new sizevec_t(sel);
    

    //*sv = *sdsv;
    //*sv = std::addressof(sel);

}

chooser_base_t::cbres_t idaapi ponce_table_chooser_t::enter(sizevec_t* sel)
{
    //sv = *sel;
   // *sv = *sel;
    return cbres_t(callui(ui_chooser_default_enter, this, sel).i);
}