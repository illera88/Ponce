//! \file
/*
**  Copyright (c) 2016 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

//IDA
#include <pro.h>
#include <idp.hpp>
#include <loader.hpp>
#include <kernwin.hpp>

//Triton
#include <api.hpp>

//Ponce
#include "formTaintSymbolizeInput.hpp"
#include "globals.hpp"
#include "utils.hpp"

/*Function to show a dialog to the user asking for an address and a size to taint/symbolize.
It returns a MemoryAccess with the memory address and the size indicated. the caller need to free this object*/
bool prompt_window_taint_symbolize(ea_t address, ea_t *selection_start, ea_t *selection_end)
{
	sval_t size = 1;
#ifdef __IDA70__
	if (ask_form(formTaintSymbolizeInput,
		NULL,
		&address,
		&size
		) > 0)
	{
		*selection_start = address;
		*selection_end = address + size;
		return true;
	}
#else
	if (AskUsingForm_c(formTaintSymbolizeInput,
		NULL,
		&address,
		&size
		) > 0)
	{
		*selection_start = address;
		*selection_end = address + size;
		return true;
	}
#endif
	return false;
}