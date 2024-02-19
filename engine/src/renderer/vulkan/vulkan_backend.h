#pragma once

#include "renderer/renderer_backend.h"

Boolean vulkan_renderer_backend_initialize(renderer_backend* backend, const char* application_name);
void vulkan_renderer_backend_shutdown(renderer_backend* backend);

void vulkan_renderer_backend_on_resized(renderer_backend* backend, UInt16 width, UInt16 height);

Boolean vulkan_renderer_backend_begin_frame(renderer_backend* backend, Single delta_time);
void vulkan_renderer_update_global_state(mat4 projection, mat4 view, vec3 view_position, vec4 ambient_colour, Int32 mode);
Boolean vulkan_renderer_backend_end_frame(renderer_backend* backend, Single delta_time);
