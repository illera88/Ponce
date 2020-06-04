//! \file
/*
**  Copyright (c) 2020 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

#pragma once

#include <vector>
#include "kernwin.hpp"


extern struct ponce_table_chooser_t* ponce_table_chooser;

// column widths
struct ponce_table_chooser_t : public chooser_multi_t
{
protected:
    static const int widths_[];
    static const char* header_[];
    typedef struct s
    {
        triton::usize id = 0;
        triton::uint64 address = 0;
        std::string register_name;
        triton::uint512 value = 0;
        std::string comment;
        std::string var_name;
        std::string constrains;
        
    }list_item_t;
   
    TWidget* widget;
    
public:

    std::map<triton::usize, std::vector<std::tuple<triton::ast::SharedAbstractNode, std::string>>> constrains;

    virtual ~ponce_table_chooser_t() {
        if (!table_item_list.empty()) {
            table_item_list.clear();
        }
    }
    // function that returns number of lines in the list
    virtual size_t idaapi get_count() const { return table_item_list.size(); }

    // function that generates the list line
    virtual void idaapi get_row(qstrvec_t* cols, int* icon_,
        chooser_item_attrs_t* attrs,
        size_t n) const;

    // function that is called when the user wants to refresh the chooser
    virtual cbres_t idaapi refresh(sizevec_t* sel) {
        fill_entryList();
        //return adjust_last_item(n);  // try to preserve the cursor
        return ALL_CHANGED;
    }


    // function that is called when the user wants to close the chooser
    virtual void idaapi closed() {
        table_item_list.clear();
        constrains.clear();
        ponce_table_chooser = nullptr;
    }

public:
    std::vector<list_item_t> table_item_list;

    ponce_table_chooser_t();
    void fill_entryList();
};




static const char constrain_form[]=
"STARTITEM 0\n"
"Add constrain to symbolic variable(s)\n\n"
"%/"
"Set limits:\n"
"<#Lower bound restriction#Lower bound:q1:16:16>\n"
"<#Upper bound restriction#Upper bound:q2:16:16>\n"

"<#Common constraints#Custom:R3>\n"
"<#ASCII only:R4>\n"
"<#Numeric only:R5>\n"
"<#Uppercase letters only:R6>\n"
"<#Lowercase letters only:R7>>\n"

"\n"
;


int ask_constrain(sizevec_t& choser_selection, int* upper_limit_int, int* lower_limit_int);
