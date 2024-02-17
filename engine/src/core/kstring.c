#include "core/kstring.h"
#include "core/kmemory.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

UInt64 string_length(const char* str) {
    return strlen(str);
}

char* string_duplicate(const char* str) {
    UInt64 length = string_length(str);
    char* copy = kallocate(length + 1, MEMORY_TAG_STRING);
    kcopy_memory(copy, str, length + 1);
    return copy;
}

// Case-sensitive string comparison. True if the same, otherwise FALSE.
Boolean strings_equal(const char* str0, const char* str1) {
    return strcmp(str0, str1) == 0;
}

Int32 string_format(char* dest, const char* format, ...) {
    if (dest) {
        __builtin_va_list arg_ptr;
        va_start(arg_ptr, format);
        Int32 written = string_format_v(dest, format, arg_ptr);
        va_end(arg_ptr);
        return written;
    }

    return -1;
}

Int32 string_format_v(char* dest, const char* format, void* va_listp) {
    if (dest) {
        char buffer[32000];
        Int32 written = vsnprintf(buffer, 32000, format, va_listp);
        buffer[written] = 0;
        kcopy_memory(dest, buffer, written + 1);
        return written;
    }

    return -1;
}