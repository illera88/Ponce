//! \file
/*
**  Copyright (c) 2016 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

#pragma once

extern action_desc_t action_IDA_show_config;
extern action_desc_t action_IDA_show_taintWindow;
extern action_desc_t action_IDA_unload;
extern action_desc_t action_IDA_solve_formula_sub;
extern action_desc_t action_IDA_negate_and_inject;
extern action_desc_t action_IDA_taint_symbolize_register;
extern action_desc_t action_IDA_taint_symbolize_memory;
extern action_desc_t action_IDA_ponce_banner;

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