/*
*  This plugin demonstrates how to use choosers inside forms.
*
*/
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
triton::arch::MemoryAccess *prompt_window_taint_symbolize(ea_t address)
{
	sval_t size = 1;
	if (AskUsingForm_c(formTaintSymbolizeInput,
		NULL,
		&address,
		&size
		) > 0)
	{
		return new triton::arch::MemoryAccess((triton::uint64)address, (triton::uint32)size, 0);
	}
	return NULL;
}