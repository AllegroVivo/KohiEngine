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
    "UNKNOWN    ",
    "ARRAY      ",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "APPLICATION",
    "JOB        ",
    "TEXTURE    ",
    "MAT_INST   ",
    "RENDERER   ",
    "GAME       ",
    "TRANSFORM  ",
    "ENTITY     ",
    "ENTITY_NODE",
    "SCENE      "
};

static struct memory_stats stats;

void initialize_memory()
{
    platform_zero_memory(&stats, sizeof(stats));
}

void shutdown_memory()
{
}

void* kallocate(UInt64 size, memory_tag tag)
{
    if (tag == MEMORY_TAG_UNKNOWN)
        KWARN("kallocate called using MEMORY_TAG_UNKNOWN. Re-class this allocation.");
    
    stats.total_allocated += size;
    stats.tagged_allocations[tag] += size;
    
    void* memory_block = platform_allocate(size, FALSE);
    platform_zero_memory(memory_block, size);
    
    return memory_block;
}

void kfree(void* block, UInt64 size, memory_tag tag)
{
    if (tag == MEMORY_TAG_UNKNOWN)
        KWARN("kfree called using MEMORY_TAG_UNKNOWN. Re-class this allocation.");
    
    stats.total_allocated -= size;
    stats.tagged_allocations[tag] -= size;
    
    platform_free(block, FALSE);
}

void* kzero_memory(void* block, UInt64 size)
{
    return platform_zero_memory(block, size);
}

void* kcopy_memory(void* dest, const void* source, UInt64 size)
{
    return platform_copy_memory(dest, source, size);
}

void* kset_memory(void* dest, UInt32 value, UInt64 size)
{
    return platform_set_memory(dest, value, size);
}

char* get_memory_usage_str()
{
    const UInt64 GiB = 1024 * 1024 * 1024;
    const UInt64 MiB = 1024 * 1024;
    const UInt64 KiB = 1024;
    
    char buffer[8000] = "System memory used (tagged):\n";
    UInt64 offset = strlen(buffer);

    for (UInt32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        char unit[4] = "XiB";
        Single amount;
        
        if (stats.tagged_allocations[i] >= GiB) {
            unit[0] = 'G';
            amount = stats.tagged_allocations[i] / (Single)GiB;
        }
        else if (stats.tagged_allocations[i] >= MiB) {
            unit[0] = 'M';
            amount = stats.tagged_allocations[i] / (Single)MiB;
        }
        else if (stats.tagged_allocations[i] >= KiB) {
            unit[0] = 'K';
            amount = stats.tagged_allocations[i] / (Single)KiB;
        }
        else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (Single)stats.tagged_allocations[i];
        }

        Int32 length = snprintf(buffer + offset, 8000, "  %s: %.2f%s\n", memory_tag_strings[i], amount, unit);
        offset += length;
    }
    
    char* out_string = _strdup(buffer);
    return out_string;
}