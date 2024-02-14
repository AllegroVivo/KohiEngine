#pragma once

#include "core/application.h"

typedef struct game
{
    application_config app_config;
    Boolean (*initialize)(struct game* game_inst);
    Boolean (*update)(struct game* game_inst, Single delta_time);
    Boolean (*render)(struct game* game_inst, Single delta_time);
    void (*on_resize)(struct game* game_inst, UInt32 width, UInt32 height);
    void* state;
} game;