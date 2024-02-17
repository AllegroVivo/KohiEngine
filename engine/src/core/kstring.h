#pragma once

#include "defines.h"

KAPI UInt64 string_length(const char* str);

KAPI char* string_duplicate(const char* str);

KAPI Boolean strings_equal(const char* str0, const char* str1);

KAPI Int32 string_format(char* dest, const char* format, ...);

KAPI Int32 string_format_v(char* dest, const char* format, void* va_list);