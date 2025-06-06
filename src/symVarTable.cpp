//! \file
/*
**  Copyright (c) 2020 - Ponce
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
#include <triton/context.hpp>

//IDA
#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <bytes.hpp>
#include <kernwin.hpp>

//Ponce
#include "symVarTable.hpp"
#include "globals.hpp"

struct ponce_table_chooser_t* ponce_table_chooser = nullptr;

void ponce_table_chooser_t::fill_entryList() {
    //We clear the list
    table_item_list.clear();

    for (const auto& [SymVarId, SymVar] : tritonCtx.getSymbolicVariables()) {
        list_item_t list_entry;

        list_entry.id = SymVarId;
        if (SymVar->getType() == triton::engines::symbolic::variable_e::MEMORY_VARIABLE) {
            list_entry.address = SymVar->getOrigin();
            list_entry.value = tritonCtx.getConcreteMemoryValue(list_entry.address, false);
        }
        else if (SymVar->getType() == triton::engines::symbolic::variable_e::REGISTER_VARIABLE) {
            list_entry.register_name = tritonCtx.getRegister((triton::arch::register_e)SymVar->getOrigin()).getName();
            list_entry.value = tritonCtx.getConcreteRegisterValue(tritonCtx.getRegister((triton::arch::register_e)SymVar->getOrigin()), false);
        }

        list_entry.var_name = SymVar->getName();
        list_entry.comment = SymVar->getComment();       

        // fill the constrains
        for (const auto& [id, constrain] : constrains) {
            if (list_entry.id == id){
                for (const auto& [abstract_node_constrain, str_constrain] : constrain) {
                    list_entry.constrains += list_entry.constrains.empty() ? str_constrain : " ; " +str_constrain;
                }
            }
        }
        // Finally, let's push it to the vector table
        table_item_list.push_back(list_entry);
    }
}


const int ponce_table_chooser_t::widths_[] = { 
    CHCOL_DEC | 8,
    12,
    10,
    7,
    16,
    16,
    25
};

// column headers
 const char* ponce_table_chooser_t::header_[] =
{
    "SymVarId",
    "Var Name",
    "Address", // Only for memory
    "Reg Name", // Only for registry 
    "Concrete Value",
    "Comment",
    "Constrains"
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

    if (n >= table_item_list.size())
        return;

    const list_item_t& li = table_item_list[n];

    cols[0].sprnt("%lu", li.id);
    cols[1].sprnt("%s", li.var_name.c_str());
    if(!li.register_name.empty()){
        // its a register entry
        cols[3].sprnt("%s", li.register_name.c_str());
    }
    else{    
        // Its a memory entry
        cols[2].sprnt(MEM_FORMAT, li.address);
    }

    cols[4].sprnt("%s", triton::utils::toString(li.value).c_str()); // Concrete value

    if(!li.comment.empty())
        cols[5].sprnt("%s", li.comment.c_str());

    if (!li.constrains.empty())
        cols[6].sprnt("%s", li.constrains.c_str());

}


//This function is used to activate or deactivate other items in the form while using it
int idaapi check_params_cb(int fid, form_actions_t& fa)
{
    ushort isActivated = 0;
    switch (fid)
    {
    case -1: // called at the begining
        fa.get_checkbox_value(3, &isActivated); // get symbolic engine value

        break;
    case -2:
        break;

    case 3:
    {
        fa.get_checkbox_value(fid, &isActivated);
        if (isActivated) {
            fa.enable_field(1, 1);
            fa.enable_field(2, 1);
        }
        break;
    }
    case 4:
    {
        fa.get_checkbox_value(fid, &isActivated);
        if (isActivated){
            qstring lower("0x20");
            qstring upper("0x7e");
            fa.set_string_value(1, &lower);
            fa.set_string_value(2, &upper);
            fa.enable_field(1, 0);
            fa.enable_field(2, 0);
        }       
        break;
    }
    case 5:
    {
        fa.get_checkbox_value(fid, &isActivated);
        if (isActivated) {
            qstring lower("0x30");
            qstring upper("0x39");
            fa.set_string_value(1, &lower);
            fa.set_string_value(2, &upper);
            fa.enable_field(1, 0);
            fa.enable_field(2, 0);
        }
        break;
    }
    case 6:
    {
        fa.get_checkbox_value(fid, &isActivated);
        if (isActivated) {
            qstring lower("0x41");
            qstring upper("0x5a");
            fa.set_string_value(1, &lower);
            fa.set_string_value(2, &upper);
            fa.enable_field(1, 0);
            fa.enable_field(2, 0);
        }
        break;
    }
    case 7:
    {
        fa.get_checkbox_value(fid, &isActivated);
        if (isActivated) {
            qstring lower("0x61");
            qstring upper("0x7a");
            fa.set_string_value(1, &lower);
            fa.set_string_value(2, &upper);
            fa.enable_field(1, 0);
            fa.enable_field(2, 0);
        }
        break;
    }
    default:
        break;
    }

    // Let's deactivate all the check

    return 1;
}

int ask_constrain(sizevec_t& choser_selection, int* upper_limit_int, int* lower_limit_int) {
    qstring upper_limit = "None";
    qstring lower_limit = "None";

    bool lower_set = false, upper_set = false;
    ushort chkgroup1 = 0;
    if (ask_form(constrain_form,
        check_params_cb, // the call to this function can be omitted. It's only usefull if a checkbox activate or dissable other elements of the form
        &lower_limit,
        &upper_limit,
        &chkgroup1
    ) > 0)
    {
        if (upper_limit != "None" && !upper_limit.empty()) {
            try {
                int base = 10;
                char* ptr = (char*)upper_limit.c_str();
                if (upper_limit.substr(0, 2) == "0x") {
                    // its hex
                    base = 16;
                    ptr += 2;
                }
                *upper_limit_int = std::stoi(ptr, 0, base);
                upper_set = true;
            }
            catch (...) {}
        }
        if (lower_limit != "None" && !lower_limit.empty()) {
            try{
                int base = 10;
                char* ptr = (char*)lower_limit.c_str();
                if (lower_limit.substr(0, 2) == "0x") {
                    // its hex
                    base = 16;
                    ptr += 2;
                }
                *lower_limit_int = std::stoi(ptr, 0, base);
                lower_set = true;
            }
            catch (...) {}
        }

        if (lower_set && upper_set)
            return 2;
        else if (lower_set)
            return 1;
        else if (upper_set)
            return 0;
    }
    return -1;
}