#include "game.h"

#include <core/logger.h>
#include <core/input.h>
#include <core/kmemory.h>

Boolean game_initialize(game* game_inst) {
    KDEBUG("game_initialize() called!");
    return TRUE;
}

Boolean game_update(game* game_inst, Single delta_time) {

    static UInt64 alloc_count = 0;
    UInt64 prev_alloc_count = alloc_count;
    alloc_count = get_memory_alloc_count();
    if (input_is_key_up('M') && input_was_key_down('M')) {
        KDEBUG("Allocations: %llu (%llu this frame).", alloc_count, alloc_count - prev_alloc_count);
    } 

    return TRUE;
}

Boolean game_render(game* game_inst, Single delta_time) {
    return TRUE;
}

void game_on_resize(game* game_inst, UInt32 width, UInt32 height) {
    
}