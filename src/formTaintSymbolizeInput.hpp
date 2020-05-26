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
//Triton
#include <triton/api.hpp>

//IDA
#include <ida.hpp>

bool prompt_window_taint_symbolize(ea_t address, sval_t size, ea_t* selection_start, ea_t* selection_end);

static const char formTaintSymbolizeInput[] =
"STARTITEM 1\n"
"Symbolize/Taint memory range\n\n"
"%/"
"<#The memory address in hex#Address\t:M1:16:16>\n"
"<#The size#Size   \t:D2:16:16>\n"
"\n"
;