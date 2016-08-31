#include <string>
//Triton
#include <api.hpp>
//IDA
#include <idp.hpp>
#include <loader.hpp>

/*This functions gets a string and return the triton register assign or NULL
This is using the triton current architecture so it is more generic.*/
triton::arch::Register *str_to_register(std::string register_name)
{
	auto regs = triton::api.getAllRegisters();
	for (auto it = regs.begin(); it != regs.end(); it++)
	{
		triton::arch::Register *reg = *it;
		if (reg->getName() == register_name)
			return reg;
	}
	return NULL;
}

/*We need this helper because triton doesn't allow to taint memory regions unalinged, so we taint every byte*/
void taint_all_memory(triton::__uint address, triton::__uint size)
{
	for (unsigned int i = 0; i < size; i++)
	{
		triton::api.taintMemory(address + i);
	}
}

/*This function ask to the user to take a snapshot.
It returns:
1: yes
0: No
-1: Cancel execution of script*/
int ask_for_a_snapshot()
{
	while (true)
	{
		int answer = askyn_c(1, "[? ] Do you want to take a database snapshot before using the script? (It will color some intructions) (Y/n):");
		if (answer == 1) //Yes
		{
			snapshot_t snapshot;
			strcpy_s(snapshot.desc, MAX_DATABASE_DESCRIPTION, "Before use the script");
			qstring errmsg;
			bool success = take_database_snapshot(&snapshot, &errmsg);
			return 1;
		}
		else if (answer == 0) //No
			return 0;
		else //Cancel
			return -1;
	}
}