#pragma once

#include "defines.h"

#include "math/math_types.h"

typedef enum renderer_backend_type {
    RENDERER_BACKEND_TYPE_VULKAN,
    RENDERER_BACKEND_TYPE_OPENGL,
    RENDERER_BACKEND_TYPE_DIRECTX
} renderer_backend_type;

typedef struct global_uniform_object {
    mat4 projection;
    mat4 view;
    mat4 m_reserved0;
    mat4 m_reserved1;
} global_uniform_object;

typedef struct renderer_backend {
    UInt64 frame_number;

    Boolean (*initialize)(struct renderer_backend* backend, const char* application_name);
    void (*shutdown)(struct renderer_backend* backend);
    void (*resized)(struct renderer_backend* backend, UInt16 width, UInt16 height);
    Boolean (*begin_frame)(struct renderer_backend* backend, Single delta_time);
    void (*update_global_state)(mat4 projection, mat4 view, vec3 view_position, vec4 ambient_colour, Int32 mode);
    Boolean (*end_frame)(struct renderer_backend* backend, Single delta_time);
} renderer_backend;


typedef struct render_packet {
    Single delta_time;
} render_packet;
