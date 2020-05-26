//! \file
/*
**  Copyright (c) 2020 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

#pragma once

#include <idp.hpp>

ssize_t idaapi tracer_callback(void* /*user_data*/, int notification_code, va_list va);
ssize_t idaapi ui_callback(void* /*ud*/, int notification_code, va_list va);
