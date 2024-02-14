#pragma once

#include "renderer_types.inl"

struct static_mesh_data;
struct platform_state;

Boolean renderer_initialize(const char* application_name, struct platform_state* plat_state);
void renderer_shutdown();

void renderer_on_resized(UInt16 width, UInt16 height);

Boolean renderer_draw_frame(render_packet* packet);