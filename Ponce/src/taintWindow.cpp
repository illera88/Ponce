

//Triton
#include <api.hpp>


//IDA
#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <bytes.hpp>
#include <kernwin.hpp>

//Ponce
#include "taintWindow.hpp"
#include "globals.hpp"

// function that returns number of lines in the list
static ulong idaapi sizer(void *obj)
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
	//  warning("destroy_cb");
	/*netnode *node = (netnode *)obj;
	node->kill();
	delete node;*/
}


// function that generates the list line
static void idaapi desc(void *obj, ulong n, char * const *arrptr)
{
	if (n == 0) // generate the column headers
	{
		for (int i = 0; i < qnumber(header); i++)
			qstrncpy(arrptr[i], header[i], MAXSTR);
		return;
	}
	n--;
	entrylist_t &li = *(entrylist_t *)obj;
	item_t;

	qsnprintf(arrptr[0], MAXSTR, "%u", li[n].id);
	qsnprintf(arrptr[1], MAXSTR, "%08a", li[n].address);
	qsnprintf(arrptr[2], MAXSTR, "%s", li[n].register_name);
	qsnprintf(arrptr[3], MAXSTR, "%llu", li[n].value);
	qsnprintf(arrptr[4], MAXSTR, "%s", li[n].isTainted ? "true" : "false");
	qsnprintf(arrptr[5], MAXSTR, "%s", li[n].isSymbolized ? "true" : "false");
	qsnprintf(arrptr[6], MAXSTR, "%s", li[n].comment);
	
}

entrylist_t * fill_entryList(){
	entrylist_t *li = new entrylist_t;

	if (cmdOptions.use_tainting_engine){
		auto taintedMemoryList = triton::api.getTaintEngine()->getTaintedMemory();
		for (auto iterator = taintedMemoryList.begin(); iterator != taintedMemoryList.end(); ++iterator) {
			item_t list_entry;
			list_entry.address = *iterator;
			list_entry.isTainted = true;
			list_entry.value = triton::api.getConcreteMemoryValue(*iterator);
		}
	}
	else if (cmdOptions.use_symbolic_engine){
		auto symMemMap = triton::api.getSymbolicMemory();
		auto symRegMap = triton::api.getSymbolicRegisters();
		triton::api.getSymbolicEngine();

		//Iterate over symbolic memory
		typedef std::map<triton::uint64, triton::engines::symbolic::SymbolicExpression*>::iterator it_type;
		for (it_type iterator = symMemMap.begin(); iterator != symMemMap.end(); iterator++) {
			auto symbExpr = iterator->second;

			item_t list_entry;
			list_entry.isSymbolized = symbExpr->isSymbolized();
			list_entry.id = symbExpr->getId();
			list_entry.address = symbExpr->getOriginMemory().getAddress();
			list_entry.comment = symbExpr->getComment().c_str();
			list_entry.value = symbExpr->getOriginMemory().getConcreteValue();

			li->push_back(list_entry);
		}

		//Iterate over symbolic registers
		
		//typedef std::map<triton::arch::Register, triton::engines::symbolic::SymbolicExpression*>::iterator it_type;
		for (auto iterator = symMemMap.begin(); iterator != symMemMap.end(); iterator++) {
			auto symbExpr = iterator->second;
			item_t list_entry;

			list_entry.isSymbolized = symbExpr->isSymbolized();
			list_entry.id = symbExpr->getId();
			list_entry.register_name = symbExpr->getOriginRegister().getName().c_str();
			list_entry.comment = symbExpr->getComment().c_str();
			list_entry.value = symbExpr->getOriginRegister().getConcreteValue();

			li->push_back(list_entry);
		}
	}
	return li;
}


void create_taint_window(){
	auto *li = fill_entryList();

	// now open the window
	choose2(false,                    // non-modal window
		-1, -1, -1, -1,       // position is determined by Windows
		li,                 // pass the created array
		qnumber(header),      // number of columns
		widths,               // widths of columns
		sizer,                // function that returns number of lines
		desc,                 // function that generates a line
		"windows title",                // window title
		-1,                   // use the default icon for the window
		0,                    // position the cursor on the first line
		NULL,                 // "kill" callback
		NULL,                 // "new" callback
		NULL,                 // "update" callback
		NULL,                 // "edit" callback
		enter_cb,             // function to call when the user pressed Enter
		destroy_cb,           // function to call when the window is closed
		NULL,                 // use default popup menu items
		NULL);                // use the same icon for all lines

	qfree(li);
}

