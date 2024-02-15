#pragma once

#include "renderer/renderer_backend.h"

Boolean vulkan_renderer_backend_initialize(renderer_backend* backend, const char* application_name, struct platform_state* plat_state);
void vulkan_renderer_backend_shutdown(renderer_backend* backend);

void vulkan_renderer_backend_on_resized(renderer_backend* backend, UInt16 width, UInt16 height);

Boolean vulkan_renderer_backend_begin_frame(renderer_backend* backend, Single delta_time);
Boolean vulkan_renderer_backend_end_frame(renderer_backend* backend, Single delta_time);
