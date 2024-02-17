#pragma once

#include "defines.h"

typedef enum renderer_backend_type {
    RENDERER_BACKEND_TYPE_VULKAN,
    RENDERER_BACKEND_TYPE_OPENGL,
    RENDERER_BACKEND_TYPE_DIRECTX
} renderer_backend_type;

typedef struct renderer_backend {
    UInt64 frame_number;

    Boolean (*initialize)(struct renderer_backend* backend, const char* application_name);
    void (*shutdown)(struct renderer_backend* backend);
    void (*resized)(struct renderer_backend* backend, UInt16 width, UInt16 height);
    Boolean (*begin_frame)(struct renderer_backend* backend, Single delta_time);
    Boolean (*end_frame)(struct renderer_backend* backend, Single delta_time);
} renderer_backend;


typedef struct render_packet {
    Single delta_time;
} render_packet;
