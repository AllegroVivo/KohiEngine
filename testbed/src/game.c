#include "game.h"

#include <core/logger.h>

Boolean game_initialize(game* game_inst) {
    KDEBUG("game_initialize() called!");
    return TRUE;
}

Boolean game_update(game* game_inst, Single delta_time) {
    return TRUE;
}

Boolean game_render(game* game_inst, Single delta_time) {
    return TRUE;
}

void game_on_resize(game* game_inst, UInt32 width, UInt32 height) {
    
}