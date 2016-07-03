#pragma once

#include <idp.hpp>

int idaapi tracer_callback(void * /*user_data*/, int notification_code, va_list va);
int idaapi ui_callback(void * /*ud*/, int notification_code, va_list va);