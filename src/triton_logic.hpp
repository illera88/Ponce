#pragma once

#include <dbg.hpp>

int tritonize(ea_t pc, thid_t threadID = 0);
void triton_restart_engines();
void start_tainting_or_symbolic_analysis();
bool ponce_set_triton_architecture();