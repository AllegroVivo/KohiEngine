#pragma once

#include "defines.h"

typedef struct linear_allocator {
    UInt32 total_size;
    UInt64 allocated;
    void* memory;
    Boolean owns_memory;
} linear_allocator;

KAPI void linear_allocator_create(UInt64 total_size, void* memory, linear_allocator* out_allocator);
KAPI void linear_allocator_destroy(linear_allocator* allocator);

KAPI void* linear_allocator_allocate(linear_allocator* allocator, UInt64 size);
KAPI void linear_allocator_free_all(linear_allocator* allocator);