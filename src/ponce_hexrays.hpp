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

#ifdef BUILD_HEXRAYS_SUPPORT
#include <hexrays.hpp>
#if IDA_SDK_VERSION == 700
int idaapi ponce_hexrays_callback(void*, hexrays_event_t event, va_list va);
#else
ssize_t idaapi ponce_hexrays_callback(void*, hexrays_event_t event, va_list va);
#endif
#endif

extern bool hexrays_present;