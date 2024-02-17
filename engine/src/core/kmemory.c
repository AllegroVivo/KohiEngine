#include "kmemory.h"
#include "logger.h"
#include "platform/platform.h"

#include <string.h>
#include <stdio.h>

struct memory_stats
{
    UInt32 total_allocated;
    UInt32 tagged_allocations[MEMORY_TAG_MAX_TAGS];
};

static const char* memory_tag_strings[MEMORY_TAG_MAX_TAGS] =
{
    "UNKNOWN     ",
    "ARRAY       ",
    "LINEAR ALLOC",
    "DARRAY      ",
    "DICT        ",
    "RING_QUEUE  ",
    "BST         ",
    "STRING      ",
    "APPLICATION ",
    "JOB         ",
    "TEXTURE     ",
    "MAT_INST    ",
    "RENDERER    ",
    "GAME        ",
    "TRANSFORM   ",
    "ENTITY      ",
    "ENTITY_NODE ",
    "SCENE       "
};

typedef struct memory_system_state {
    struct memory_stats stats;
    UInt64 alloc_count;
} memory_system_state;

static memory_system_state* state_ptr;

void initialize_memory(UInt64* memory_requirement, void* state) {
    *memory_requirement = sizeof(memory_system_state);
    
    if (state == 0) {
        return;
    }

    state_ptr = state;
    state_ptr->alloc_count = 0;
    platform_zero_memory(&state_ptr->stats, sizeof(state_ptr->stats));
}

void shutdown_memory(void* state) {
    state_ptr = 0;
}

void* kallocate(UInt64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN)
        KWARN("kallocate called using MEMORY_TAG_UNKNOWN. Re-class this allocation.");
    
    if (state_ptr) {
        state_ptr->stats.total_allocated += size;
        state_ptr->stats.tagged_allocations[tag] += size;
        state_ptr->alloc_count++;
    }
    
    void* memory_block = platform_allocate(size, FALSE);
    platform_zero_memory(memory_block, size);
    
    return memory_block;
}

void kfree(void* block, UInt64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN)
        KWARN("kfree called using MEMORY_TAG_UNKNOWN. Re-class this allocation.");
    
    state_ptr->stats.total_allocated -= size;
    state_ptr->stats.tagged_allocations[tag] -= size;
    
    platform_free(block, FALSE);
}

void* kzero_memory(void* block, UInt64 size) {
    return platform_zero_memory(block, size);
}

void* kcopy_memory(void* dest, const void* source, UInt64 size) {
    return platform_copy_memory(dest, source, size);
}

void* kset_memory(void* dest, UInt32 value, UInt64 size) {
    return platform_set_memory(dest, value, size);
}

char* get_memory_usage_str() {
    const UInt64 GiB = 1024 * 1024 * 1024;
    const UInt64 MiB = 1024 * 1024;
    const UInt64 KiB = 1024;
    
    char buffer[8000] = "System memory used (tagged):\n";
    UInt64 offset = strlen(buffer);

    for (UInt32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        char unit[4] = "XiB";
        Single amount;
        
        if (state_ptr->stats.tagged_allocations[i] >= GiB) {
            unit[0] = 'G';
            amount = state_ptr->stats.tagged_allocations[i] / (Single)GiB;
        }
        else if (state_ptr->stats.tagged_allocations[i] >= MiB) {
            unit[0] = 'M';
            amount = state_ptr->stats.tagged_allocations[i] / (Single)MiB;
        }
        else if (state_ptr->stats.tagged_allocations[i] >= KiB) {
            unit[0] = 'K';
            amount = state_ptr->stats.tagged_allocations[i] / (Single)KiB;
        }
        else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (Single)state_ptr->stats.tagged_allocations[i];
        }

        Int32 length = snprintf(buffer + offset, 8000, "  %s: %.2f%s\n", memory_tag_strings[i], amount, unit);
        offset += length;
    }
    
    char* out_string = _strdup(buffer);
    return out_string;
}

UInt64 get_memory_alloc_count() {
    if (state_ptr) {
        return state_ptr->alloc_count;
    }
    return 0;
}