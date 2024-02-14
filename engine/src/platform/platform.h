#pragma once

#include "defines.h"

typedef struct platform_state {
    void* internal_state;
} platform_state;

KAPI Boolean platform_startup(
    platform_state* platform_state,
    const char* application_name,
    Int32 x,
    Int32 y,
    Int32 width,
    Int32 height);

KAPI void platform_shutdown(platform_state* platform_state);

KAPI Boolean platform_pump_messages(platform_state* platform_state);

void* platform_allocate(UInt64 size, Boolean aligned);
void platform_free(void* block, Boolean aligned);
void* platform_zero_memory(void* block, UInt64 size);
void* platform_copy_memory(void* dest, const void* source, UInt64 size);
void* platform_set_memory(void* dest, Int32 value, UInt64 size);

void platform_console_write(const char* message, UInt8 color);
void platform_console_write_error(const char* message, UInt8 color);

Double platform_get_absolute_time();

void platform_sleep(UInt64 ms);