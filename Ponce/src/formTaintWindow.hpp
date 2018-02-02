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
#include <vector>
#include "kernwin.hpp"

struct item_t
{
	triton::usize id = 0;
	triton::uint64 address = 0;
	std::string register_name;
	triton::uint512 value = 0;
	bool isTainted = false;
	bool isSymbolized = false;
	std::string comment;
};

typedef std::vector<item_t*> entrylist_t;

void create_taint_window();
void fill_entryList();

#ifndef __IDA70__

// column widths
static const int widths[] = { CHCOL_DEC | 8,
CHCOL_HEX,
5,
16,
6,
6,
16 };

// column headers
static const char *header[] =
{
	"Id",
	"Address",
	"Reg Name",
	"Value",
	"Is Tainted",
	"Is Symbolized",
	"Comment",
};
#endif
