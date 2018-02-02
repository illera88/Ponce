//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the BSD License.
*/

#include <iostream>
#include "snapshot.hpp"

#include "globals.hpp"
#include "dbg.hpp"

/*! \page Snapshot_page Snapshot
\brief [**pintool internal**] All information about the snapshot engine.

\tableofcontents
\section snapshot_description Description
<hr>

The pintool shipped in this project (see \ref Tracer_page) allows the user to replay a trace. During the execution, it is possible to
take a snapshot of the registers and memory states. Then, at each program point, it is possible to restore the previous snapshot.

<b>Example</b>: Imagine a trace with a `LOAD` value, this value is controllable by the user. Then, some operations are applied to this value
and at the end the value is verified with a constant. At the compare instruction the formula of the operation applied to the value is
built, by assigning a symbolic variable to the input value, it is possible to solve the formula (if it is satisfiable). So, it will
be useful to directly inject the model returned by the solver in memory instead of re-run the program.

<p align="center"><img src="http://triton.quarkslab.com/files/triton_snapshot.svg"/></p>


As taking a snapshot of the full memory is not really possible, the pintool saves all bytes before each modification of the memory (`STORE` access)
in a map \f$ \langle addr, byte \rangle \f$.

When the snapshot must be restored, all modifications are re-injected in memory like this: \f$ \forall (addr, byte \in map) : *addr \gets byte \f$

*/


Snapshot::Snapshot() {
	this->locked = true;
	this->snapshotTaintEngine = nullptr;
	this->snapshotSymEngine = nullptr;
	this->mustBeRestore = false;
	this->snapshotTaken = false;
}


Snapshot::~Snapshot() {
}

/* Check if the snapshot has been taken */
bool Snapshot::exists(void) {
	return this->snapshotTaken;
}

/* Add the modification byte. */
void Snapshot::addModification(ea_t mem, char byte) {
	if (this->locked == false && this->memory.find(mem) == this->memory.end())
		this->memory[mem] = byte;
}


/* Enable the snapshot engine. */
void Snapshot::takeSnapshot() {
	this->snapshotTaken = true;

	/* 1 - Unlock the engine */
	this->locked = false;

	/* 2 - Save current symbolic engine state */
	this->snapshotSymEngine = new triton::engines::symbolic::SymbolicEngine(*triton::api.getSymbolicEngine());

	/* 3 - Save current taint engine state */
	this->snapshotTaintEngine = new triton::engines::taint::TaintEngine(*triton::api.getTaintEngine());

	/* 4 - Save current set of nodes */
	this->nodesList = triton::api.getAllocatedAstNodes();

	/* 5 - Save current map of variables */
	this->variablesMap = triton::api.getAstVariableNodes();

	/* 6 - Save the Triton CPU state */
#if defined(__x86_64__) || defined(_M_X64)
	this->cpu = new triton::arch::x86::x8664Cpu(*reinterpret_cast<triton::arch::x86::x8664Cpu*>(triton::api.getCpu()));
#endif
#if defined(__i386) || defined(_M_IX86)
	this->cpu = new triton::arch::x86::x86Cpu(*reinterpret_cast<triton::arch::x86::x86Cpu*>(triton::api.getCpu()));
#endif

	/* 6 - Save IDA registers context */
	auto regs = triton::api.getAllRegisters();
	for (auto it = regs.begin(); it != regs.end(); it++)
	{
		triton::arch::Register *reg = *it;
		uint64 ival;
		if (get_reg_val(reg->getName().c_str(), &ival)){
			this->IDAContext[reg->getName()] = ival;
		}
	}

	//We also saved the ponce status
	this->saved_ponce_runtime_status = ponce_runtime_status;
}

void Snapshot::setAddress(ea_t address){
	this->address = address;
}


/* Restore the snapshot. */
void Snapshot::restoreSnapshot() {

	/* 1 - Restore all memory modification. */
	for (auto i = this->memory.begin(); i != this->memory.end(); ++i){
#ifdef __IDA70__
		put_bytes(i->first, &i->second, 1);
#else
		put_many_bytes(i->first, &i->second, 1);
#endif
	}
	this->memory.clear();

	/* 2 - Restore current symbolic engine state */
	*triton::api.getSymbolicEngine() = *this->snapshotSymEngine;

	/* 3 - Restore current taint engine state */
	*triton::api.getTaintEngine() = *this->snapshotTaintEngine;

	/* 4 - Restore current AST node state */
	triton::api.setAllocatedAstNodes(this->nodesList);

	/* 5 - Restore current variables map state */
	triton::api.setAstVariableNodes(this->variablesMap);

	/* 6 - Restore the Triton CPU state */
#if defined(__x86_64__) || defined(_M_X64)
	*reinterpret_cast<triton::arch::x86::x8664Cpu*>(triton::api.getCpu()) = *this->cpu;
#endif
#if defined(__i386) || defined(_M_IX86)
	*reinterpret_cast<triton::arch::x86::x86Cpu*>(triton::api.getCpu()) = *this->cpu;
#endif

	this->mustBeRestore = false;

	/* 7 - Restore IDA registers context 
	Suposedly XIP should be set at the same time and execution redirected*/
	typedef std::map<std::string, triton::uint512>::iterator it_type;
	for (it_type iterator = this->IDAContext.begin(); iterator != this->IDAContext.end(); iterator++) {
		if (!set_reg_val(iterator->first.c_str(), iterator->second.convert_to<uint64>()))
			msg("[!] ERROR restoring register %s\n", iterator->first.c_str());
	}

	/* 8 - Restore the Ponce status */
	ponce_runtime_status = this->saved_ponce_runtime_status;

	/* 9 - We need to set to NULL the last instruction. We are deleting the last instructions in the Tritonize callback.
	So after restore a snapshot if last_instruction is not NULL is double freeing the same instruction */
	ponce_runtime_status.last_triton_instruction = NULL;
}

/* Disable the snapshot engine. */
void Snapshot::disableSnapshot(void) {
	this->locked = true;
}


/* Reset the snapshot engine.
* Clear all backups for a new snapshot. */
void Snapshot::resetEngine(void) {
	this->memory.clear();

	//ToDo: We should delete this when this issue is fixed: https://github.com/JonathanSalwan/Triton/issues/385
	delete this->snapshotSymEngine;
	this->snapshotSymEngine = nullptr;

	delete this->snapshotTaintEngine;
	this->snapshotTaintEngine = nullptr;

	this->snapshotTaken = false;

	//We delete the comment that we created
	set_cmt(this->address, "", false);
	this->address = 0;
}


/* Check if the snapshot engine is locked. */
bool Snapshot::isLocked(void) {
	return this->locked;
}


/* Check if we must restore the snapshot */
bool Snapshot::mustBeRestored(void) {
	return this->mustBeRestore;
}


/* Check if we must restore the snapshot */
void Snapshot::setRestore(bool flag) {
	this->mustBeRestore = flag;
}
