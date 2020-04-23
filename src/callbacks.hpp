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

#include <idp.hpp>
#include <list>

#include "globals.hpp"

ssize_t idaapi tracer_callback(void * /*user_data*/, int notification_code, va_list va);
ssize_t idaapi ui_callback(void * /*ud*/, int notification_code, va_list va);

int tritonize(ea_t pc, thid_t threadID = 0);
void set_SMT_results(Input* input_ptr);
void triton_restart_engines();