
#pragma once
#include <vector>
#include "kernwin.hpp"

// column widths
static const int widths[] = { CHCOL_DEC, 
CHCOL_HEX | 8, 
32, 
32, 
32, 
32,
32 };

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

struct item_t
{
	triton::usize id = NULL;

	triton::uint64 address = NULL;
	const char* register_name = NULL;

	triton::uint512 value = 0;

	bool isTainted = false;
	bool isSymbolized = false;

	const char* comment = NULL; 
};

typedef std::vector<item_t> entrylist_t;

void create_taint_window();