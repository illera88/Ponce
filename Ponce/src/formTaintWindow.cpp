//! \file
/*
**  Copyright (c) 2016 - Ponce
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
#include <api.hpp>

//IDA
#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <bytes.hpp>
#include <kernwin.hpp>

//Ponce
#include "formTaintWindow.hpp"
#include "globals.hpp"

const char *popup_menu_names[] = {
	"Insert",					// not active
	"Delete",					// not active
	"Dump segment to disk",		// menu entry for dumping the segment
	"Refresh"					// not active
};

entrylist_t *global_list;

#ifdef __IDA70__



void fill_entryList() {

	//We clear the list
	global_list->clear();

	if (cmdOptions.use_tainting_engine){
		auto taintedMemoryList = triton::api.getTaintedMemory();
		auto taintedRegistersList = triton::api.getTaintedRegisters();

		//Iterate over tainted memory
		for (auto iterator = taintedMemoryList.begin(); iterator != taintedMemoryList.end(); ++iterator) {
			item_t *list_entry = new item_t();

			list_entry->address = *iterator;
			list_entry->isTainted = true;
			list_entry->value = triton::api.getConcreteMemoryValue(*iterator);

			global_list->push_back(list_entry);
		}

		//Iterate over tainted registers
		for (auto iterator = taintedRegistersList.begin(); iterator != taintedRegistersList.end(); ++iterator) {
			item_t *list_entry = new item_t();

			list_entry->register_name = (*iterator).getName();
			list_entry->isTainted = true;
			list_entry->value = (*iterator).getConcreteValue();

			global_list->push_back(list_entry);
		}
	}
	else if (cmdOptions.use_symbolic_engine){
		auto symMemMap = triton::api.getSymbolicMemory();
		auto symRegMap = triton::api.getSymbolicRegisters();
		triton::api.getSymbolicEngine();

		//Iterate over symbolic memory
		for (auto iterator = symMemMap.begin(); iterator != symMemMap.end(); iterator++) {
			auto symbExpr = iterator->second;
			item_t *list_entry = new item_t();

			list_entry->isSymbolized = symbExpr->isSymbolized();
			list_entry->id = symbExpr->getId();
			list_entry->address = iterator->first;
			list_entry->comment = symbExpr->getComment();
			list_entry->value = symbExpr->getOriginMemory().getConcreteValue();

			global_list->push_back(list_entry);
		}

		//Iterate over symbolic registers
		for (auto iterator = symRegMap.begin(); iterator != symRegMap.end(); iterator++) {
			auto symbExpr = iterator->second;
			auto reg = symbExpr->getOriginRegister();
			item_t *list_entry = new item_t();

			list_entry->isSymbolized = symbExpr->isSymbolized();
			list_entry->id = symbExpr->getId();
			list_entry->register_name = reg.getName();
			list_entry->comment = symbExpr->getComment();
			list_entry->value = reg.getConcreteValue();

			global_list->push_back(list_entry);
		}
	}
}

// column widths
struct entry_chooser_t : public chooser_t
{
protected:
	static const int widths_[];
	static const char *const header_[];
public:

	// this object must be allocated using `new`
	entry_chooser_t();
	virtual ~entry_chooser_t() {
		if (global_list != NULL) {
			global_list->clear();
			qfree(global_list);
			global_list = NULL;
		}
	}
	// function that returns number of lines in the list
	virtual size_t idaapi get_count() const { return global_list->size(); }

	// function that generates the list line
	virtual void idaapi get_row(qstrvec_t *cols, int *icon_,
		chooser_item_attrs_t *attrs,
		size_t n) const;

	// function that is called when the user wants to refresh the chooser
	virtual cbret_t idaapi refresh(ssize_t n) {
		fill_entryList();
		return adjust_last_item(n);  // try to preserve the cursor
	}

	// function that is called when the user wants to close the chooser
	virtual cbret_t idaapi destroy(ssize_t n) {
		if (global_list != NULL) {
			global_list->clear();
			qfree(global_list);
			global_list = NULL;
		}
		return adjust_last_item(n);  // try to preserve the cursor
	}
};

const int entry_chooser_t::widths_[] = { CHCOL_DEC | 8,
CHCOL_HEX,
5,
16,
6,
6,
16 };

// column headers
const char *const entry_chooser_t::header_[] =
{
	"Id",
	"Address",
	"Reg Name",
	"Value",
	"Is Tainted",
	"Is Symbolized",
	"Comment",
};

inline entry_chooser_t::entry_chooser_t()
	: chooser_t(CH_CAN_REFRESH, qnumber(widths_), widths_, header_, "Taint/Symbolic Items") {
	CASSERT(qnumber(widths_) == qnumber(header_));

	if (global_list == NULL) {
		global_list = new entrylist_t();
	}
	//Fill the list with Triton info
	fill_entryList();
}

// function that generates the list line
void idaapi entry_chooser_t::get_row(qstrvec_t *cols_, int *, chooser_item_attrs_t *,
	size_t n) const {
	qstrvec_t &cols = *cols_;

	item_t *li = global_list->at(n);

	cols[0].sprnt("%d", li->id);
	if (li->address == 0)
		cols[1].sprnt("%s", "");
	else
		cols[1].sprnt("0x%x", li->address);
	cols[2].sprnt("%s", li->register_name);
	cols[3].sprnt("0x%x", li->value.convert_to<ea_t>());
	cols[4].sprnt("%s", li->isTainted ? "true" : "false");
	cols[5].sprnt("%s", li->isSymbolized ? "true" : "false");
	cols[6].sprnt("%s", li->comment);

}


void create_taint_window() {
	// open the window
	entry_chooser_t *ch = new entry_chooser_t();
	ch->choose();
}

#else

// function that returns number of lines in the list
static uint32 idaapi sizer(void *obj)
{
	entrylist_t &li = *(entrylist_t *)obj;
	return li.size();
}

//-------------------------------------------------------------------------
// function that is called when the user hits Enter
static void idaapi enter_cb(void *obj, uint32 n)
{
	//netnode *node = (netnode *)obj;
	//jumpto(node->altval(n - 1));
}

//-------------------------------------------------------------------------
// function that is called when the window is closed
static void idaapi destroy_cb(void *obj)
{
	if (global_list != NULL){
		global_list->clear();
		qfree(global_list);
		global_list = NULL;
	}
}


// function that generates the list line
static void idaapi desc(void *obj, uint32 n, char * const *arrptr)
{
	if (n == 0) // generate the column headers
	{
		for (int i = 0; i < qnumber(header); i++)
			qstrncpy(arrptr[i], header[i], MAXSTR);
		return;
	}
	n--;
	entrylist_t &li = *(entrylist_t *)obj;

	qsnprintf(arrptr[0], MAXSTR, "%d", li[n]->id);
	if (li[n]->address == 0)
		qsnprintf(arrptr[1], MAXSTR, "%s", "");
	else
		qsnprintf(arrptr[1], MAXSTR, HEX_FORMAT, (ea_t)li[n]->address);
	
	qsnprintf(arrptr[2], MAXSTR, "%s", li[n]->register_name.c_str());
	qsnprintf(arrptr[3], MAXSTR, HEX_FORMAT, li[n]->value.convert_to<ea_t>());
	qsnprintf(arrptr[4], MAXSTR, "%s", li[n]->isTainted ? "true" : "false");
	qsnprintf(arrptr[5], MAXSTR, "%s", li[n]->isSymbolized ? "true" : "false");
	qsnprintf(arrptr[6], MAXSTR, "%s", li[n]->comment.c_str());
	
}

void fill_entryList(){	

	//We clear the list
	global_list->clear();

	if (cmdOptions.use_tainting_engine){
		auto taintedMemoryList = triton::api.getTaintedMemory();
		auto taintedRegistersList = triton::api.getTaintedRegisters();
		
		//Iterate over tainted memory
		for (auto iterator = taintedMemoryList.begin(); iterator != taintedMemoryList.end(); ++iterator) {
			item_t *list_entry = new item_t();

			list_entry->address = *iterator;
			list_entry->isTainted = true;
			list_entry->value = triton::api.getConcreteMemoryValue(*iterator);

			global_list->push_back(list_entry);
		}
		
		//Iterate over tainted registers
		for (auto iterator = taintedRegistersList.begin(); iterator != taintedRegistersList.end(); ++iterator) {
			item_t *list_entry = new item_t();

			list_entry->register_name = (*iterator).getName();
			list_entry->isTainted = true;
			list_entry->value = (*iterator).getConcreteValue();

			global_list->push_back(list_entry);
		}
	}
	else if (cmdOptions.use_symbolic_engine){
		auto symMemMap = triton::api.getSymbolicMemory();
		auto symRegMap = triton::api.getSymbolicRegisters();
		triton::api.getSymbolicEngine();

		//Iterate over symbolic memory
		for (auto iterator = symMemMap.begin(); iterator != symMemMap.end(); iterator++) {
			auto symbExpr = iterator->second;
			item_t *list_entry = new item_t();
			
			list_entry->isSymbolized = symbExpr->isSymbolized();
			list_entry->id = symbExpr->getId();
			list_entry->address = iterator->first;
			list_entry->comment = symbExpr->getComment();
			list_entry->value = symbExpr->getOriginMemory().getConcreteValue();

			global_list->push_back(list_entry);
		}

		//Iterate over symbolic registers
		for (auto iterator = symRegMap.begin(); iterator != symRegMap.end(); iterator++) {
			auto symbExpr = iterator->second;
			auto reg = symbExpr->getOriginRegister();
			item_t *list_entry = new item_t();
			
			list_entry->isSymbolized = symbExpr->isSymbolized();
			list_entry->id = symbExpr->getId();
			list_entry->register_name = reg.getName();
			list_entry->comment = symbExpr->getComment();
			list_entry->value = reg.getConcreteValue();

			global_list->push_back(list_entry);
		}
	}
}

static uint32 idaapi update_cb(void *obj, uint32 n)
{
	fill_entryList();
	//if (global_list != NULL){
	//	warning("freeing object");
	//	//qfree(global_list);
	//	global_list = NULL;
	//}
	//obj = (void *)global_list;
	//create_taint_window();
	return n;
}

void create_taint_window(){
	if (global_list == NULL){
		global_list = new entrylist_t();
	}
	//Fill the list with Triton info
	fill_entryList();

	// now open the window
	auto idx=choose2(false,            // non-modal window
		-1, -1, -1, -1,       // position is determined by Windows
		global_list,          // pass the created array
		qnumber(header),      // number of columns
		widths,               // widths of columns
		sizer,                // function that returns number of lines
		desc,                 // function that generates a line
		"Taint/Symbolic items",       // window title
		/*157*/-1,                   // use the default icon for the window
		-1,                    // position the cursor on the first line
		NULL,                 // "kill" callback
		NULL,                 // "new" callback
		update_cb,            // "update" callback
		NULL,                 // "edit" callback
		enter_cb,             // function to call when the user pressed Enter
		destroy_cb,           // function to call when the window is closed
		NULL,     // use default popup menu items
		NULL);                // use the same icon for all lines

}

#endif