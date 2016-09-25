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
//Triton
#include <api.hpp>

//IDA
#include <ida.hpp>

bool prompt_window_taint_symbolize(ea_t address, ea_t *selection_start, ea_t *selection_end);

static const char formTaintSymbolizeInput[] =
"STARTITEM 0\n"
"Taint / Symbolize memory range\n\n"
"%/"
"<#The memory address to taint/symbolize#Memory address\t:M1:16:16>\n"
"<#The size#Size\t:D2:10:10>\n"
"\n"
;