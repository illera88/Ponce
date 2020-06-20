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

#define __END__ -1

/* Depending on the IDA version the SDK allows or not using some of the fetures we have*/
#if IDA_SDK_VERSION < 730
const int ponce_banner_views[] = { BWN_DISASM, BWN_DUMP, BWN_CHOOSER, __END__ };
const int ponce_taint_symbolize_mem_views[] = { BWN_DISASM, BWN_DUMP, __END__ };
const int ponce_taint_symbolize_reg_views[] = { BWN_DISASM, BWN_DUMP, __END__ };
#elif IDA_SDK_VERSION == 730
const int ponce_banner_views[] = { BWN_DISASM, BWN_DUMP, BWN_STKVIEW, BWN_CHOOSER, __END__ };
const int ponce_taint_symbolize_mem_views[] = { BWN_DISASM, BWN_DUMP, BWN_STKVIEW, __END__ };
const int ponce_taint_symbolize_reg_views[] = { BWN_DISASM, BWN_DUMP, BWN_STKVIEW, __END__ };
#elif IDA_SDK_VERSION >= 740
const int ponce_banner_views[] = { BWN_DISASM, BWN_CPUREGS, BWN_DUMP, BWN_STKVIEW, BWN_CHOOSER, __END__ };
const int ponce_taint_symbolize_mem_views[] = { BWN_DISASM, BWN_CPUREGS, BWN_DUMP, BWN_STKVIEW, __END__ };
const int ponce_taint_symbolize_reg_views[] = { BWN_DISASM, BWN_CPUREGS, BWN_DUMP, BWN_STKVIEW, __END__ };
#endif

struct IDA_actions {
    const action_desc_t* action_decs;
    int view_type[10]; // This are the posible views
    char const* menu_path; // this is where the item will be conteined in
};
extern struct IDA_actions action_list[];


extern action_desc_t action_IDA_show_config;
extern action_desc_t action_IDA_show_expressionsWindow;
extern action_desc_t action_IDA_clean;
extern action_desc_t action_IDA_unload;
extern action_desc_t action_IDA_solve_formula_sub;
extern action_desc_t action_IDA_negate_and_inject;
extern action_desc_t action_IDA_negate_inject_and_restore;
extern action_desc_t action_IDA_taint_symbolize_register;
extern action_desc_t action_IDA_taint_symbolize_memory;
extern action_desc_t action_IDA_ponce_banner;
extern action_desc_t action_IDA_solve_formula_choose_index_sub;


#define SYMBOLIC "Symbolic/"
#define symbolize_REG "Symbolize Register"
#define symbolize_MEM "Symbolize Memory"
#define TAINT "Taint/"
#define TAINT_REG "Taint Register"
#define TAINT_MEM "Taint Memory"
#define COMMENT_TAINT_REG "Make the selected register tainted"
#define COMMENT_SYMB_REG "Make the selected register symbolic"
#define COMMENT_TAINT_MEM "Make the selected memory tainted"
#define COMMENT_SYMB_MEM "Make the selected memory symbolic"