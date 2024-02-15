#pragma once

#include "defines.h"

struct game;

typedef struct application_config
{
    Int16 start_pos_x;
    Int16 start_pos_y;
    Int16 start_width;
    Int16 start_height;
    char* name;
} application_config;

KAPI Boolean application_create(struct game* game_inst);
KAPI Boolean application_run();

void application_get_framebuffer_size(UInt32* width, UInt32* height);