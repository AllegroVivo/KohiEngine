#include "renderer_frontend.h"
#include "renderer_backend.h"

#include "core/logger.h"
#include "core/kmemory.h"

typedef struct renderer_system_state {
    renderer_backend backend;
} renderer_system_state;

static renderer_system_state* state_ptr;

Boolean renderer_system_initialize(UInt64* memory_requirement, void* state, const char* application_name) {
    *memory_requirement = sizeof(renderer_system_state);

    if (state == 0) {
        return TRUE;
    }

    state_ptr = state;

    renderer_backend_create(RENDERER_BACKEND_TYPE_VULKAN, &state_ptr->backend);
    state_ptr->backend.frame_number = 0;

    if (!state_ptr->backend.initialize(&state_ptr->backend, application_name)) {
        KFATAL("Renderer backend failed to initialize. Shutting down.");
        return FALSE;
    }

    return TRUE;
}

void renderer_system_shutdown(void* state) {
    if (state_ptr) {
        state_ptr->backend.shutdown(&state_ptr->backend);
    }

    state_ptr = 0;
}

Boolean renderer_begin_frame(Single delta_time) {
    if (!state_ptr) {
        return FALSE;
    }

    return state_ptr->backend.begin_frame(&state_ptr->backend, delta_time);
}

Boolean renderer_end_frame(Single delta_time) {
     if (!state_ptr) {
        return FALSE;
    }

    Boolean result = state_ptr->backend.end_frame(&state_ptr->backend, delta_time);
    state_ptr->backend.frame_number++;
    return result;
}

void renderer_on_resized(UInt16 width, UInt16 height) {
    if (state_ptr) {
        state_ptr->backend.resized(&state_ptr->backend, width, height);
    } else {
        KWARN("renderer backend does not exist to accept resize: %i %i", width, height);
    }
}

Boolean renderer_draw_frame(render_packet* packet) {
    if (renderer_begin_frame(packet->delta_time)) {
        Boolean result = renderer_end_frame(packet->delta_time);

        if (!result) {
        KERROR("renderer_end_frame failed. Application shutting down...");
        return FALSE;
        }
    }

    return TRUE;
}