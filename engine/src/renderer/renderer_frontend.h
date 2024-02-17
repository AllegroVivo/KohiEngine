#pragma once

#include "renderer_types.inl"

struct static_mesh_data;
struct platform_state;

Boolean renderer_system_initialize(UInt64* memory_requirement, void* state, const char* application_name);
void renderer_system_shutdown(void* state);

void renderer_on_resized(UInt16 width, UInt16 height);

Boolean renderer_draw_frame(render_packet* packet);