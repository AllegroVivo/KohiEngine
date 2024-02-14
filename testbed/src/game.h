#pragma once

#include <defines.h>
#include <game_types.h>

typedef struct game_state {
    Single delta_time;
} game_state;

Boolean game_initialize(game* game_inst);

Boolean game_update(game* game_inst, Single delta_time);

Boolean game_render(game* game_inst, Single delta_time);

void game_on_resize(game* game_inst, UInt32 width, UInt32 height);