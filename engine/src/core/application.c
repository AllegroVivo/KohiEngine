#include "application.h"
#include "logger.h"
#include "platform/platform.h"
#include "game_types.h"
#include "core/kmemory.h"
#include "core/event.h"
#include "core/input.h"
#include "core/clock.h"
#include "renderer/renderer_frontend.h"
#include "memory/linear_allocator.h"

typedef struct application_state
{
    game* game_inst;
    Boolean is_running;
    Boolean is_suspended;
    platform_state platform;
    Int16 width;
    Int16 height;
    clock clock;
    Double last_time;
    linear_allocator systems_allocator;

    UInt64 memory_system_memory_requirement;
    void* memory_system_state;

    UInt64 logging_system_memory_requirement;
    void* logging_system_state;
} application_state;

static application_state* app_state;

Boolean application_on_event(UInt16 code, void* sender, void* listener_inst, event_context context);
Boolean application_on_key(UInt16 code, void* sender, void* listener_inst, event_context context);
Boolean applicataion_on_resized(UInt16 code, void* sender, void* listener_inst, event_context context);

Boolean application_create(game* game_inst) {
    if (game_inst->application_state) {
        KERROR("application_create called more than once.");
        return FALSE;
    }

    game_inst->application_state = kallocate(sizeof(application_state), MEMORY_TAG_APPLICATION);
    app_state = game_inst->application_state;
    app_state->game_inst = game_inst;
    app_state->is_running = FALSE;
    app_state->is_suspended = FALSE;

    UInt64 systems_allocator_total_size = 64 * 1024 * 1024;
    linear_allocator_create(systems_allocator_total_size, 0, &app_state->systems_allocator);

    initialize_memory(&app_state->memory_system_memory_requirement, 0);
    app_state->memory_system_state = linear_allocator_allocate(&app_state->systems_allocator, app_state->memory_system_memory_requirement);
    initialize_memory(&app_state->memory_system_memory_requirement, app_state->memory_system_state);
    
    initialize_logging(&app_state->logging_system_memory_requirement, 0);
    app_state->logging_system_state = linear_allocator_allocate(&app_state->systems_allocator, app_state->logging_system_memory_requirement);
    if (!initialize_logging(&app_state->logging_system_memory_requirement, app_state->logging_system_state)) {
        KERROR("Failed to initialize logging system: shutting down.");
        return FALSE;
    }

    input_initialize();

    if (!event_initialize()) {
        KERROR("Event system failed initialization. Application cannot continue.");
        return FALSE;
    }

    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);
    event_register(EVENT_CODE_RESIZED, 0, applicataion_on_resized);
    
    if (!platform_startup(
            &app_state->platform, 
            game_inst->app_config.name, 
            game_inst->app_config.start_pos_x,
            game_inst->app_config.start_pos_y,
            game_inst->app_config.start_width, 
            game_inst->app_config.start_height)) {
        return FALSE;
    }

    if (!renderer_initialize(game_inst->app_config.name, &app_state->platform)) {
        KFATAL("Failed to initialize renderer. Aborting application.");
        return FALSE;
    }

    if (!app_state->game_inst->initialize(app_state->game_inst)) {
        KFATAL("Game failed to initialize");
        return FALSE;
    }

    app_state->game_inst->on_resize(app_state->game_inst, app_state->width, app_state->height);

    return TRUE;
}

Boolean application_run() {

    app_state->is_running = TRUE;

    clock_start(&app_state->clock);
    clock_update(&app_state->clock);
    app_state->last_time = app_state->clock.elapsed;
    Double running_time = 0;
    UInt8 frame_count = 0;
    Double target_frame_seconds = 1.f / 60;

    KINFO(get_memory_usage_str());

    while (app_state->is_running) {
        if (!platform_pump_messages(&app_state->platform)) {
            app_state->is_running = FALSE;
        }

        if (!app_state->is_suspended) {
            clock_update(&app_state->clock);
            Double current_time = app_state->clock.elapsed;
            Double delta = current_time - app_state->last_time;
            Double frame_start_time = platform_get_absolute_time();

            if (!app_state->game_inst->update(app_state->game_inst, (Single)delta)) {
                KFATAL("Game update failed, shutting down.");
                app_state->is_running = FALSE;
                break;
            }

            if (!app_state->game_inst->render(app_state->game_inst, (Single)delta)) {
                KFATAL("Game render failed, shutting down.");
                app_state->is_running = FALSE;
                break;
            }

            render_packet packet;
            packet.delta_time = delta;
            renderer_draw_frame(&packet);

            Double frame_end_time = platform_get_absolute_time();
            Double frame_elapsed_time = frame_end_time - frame_start_time;
            running_time += frame_elapsed_time;
            Double remaining_seconds = target_frame_seconds - frame_elapsed_time;

            if (remaining_seconds > 0) {
                UInt64 remaining_ms = remaining_seconds * 1000;

                Boolean limit_frames = FALSE;
                if (remaining_ms > 0 && limit_frames) {
                    platform_sleep(remaining_ms - 1);
                }

                frame_count++;
            }

            input_update(delta);

            app_state->last_time = current_time;
        }
    }

    app_state->is_running = FALSE;

    event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_unregister(EVENT_CODE_KEY_RELEASED, 0, application_on_key);
    event_unregister(EVENT_CODE_RESIZED, 0, applicataion_on_resized);

    event_shutdown();
    input_shutdown();
    renderer_shutdown();

    platform_shutdown(&app_state->platform);
    shutdown_memory();

    return TRUE;
}

void application_get_framebuffer_size(UInt32* width, UInt32* height) {
    *width = app_state->width;
    *height = app_state->height;
}

Boolean application_on_event(UInt16 code, void* sender, void* listener_inst, event_context context) {
    switch (code) {
        case EVENT_CODE_APPLICATION_QUIT: {
            KINFO("EVENT_CODE_APPLICATION_QUIT recieved, shutting down.\n");
            app_state->is_running = FALSE;
            return TRUE;
        }
    }

    return FALSE;
}

Boolean application_on_key(UInt16 code, void* sender, void* listener_inst, event_context context) {
    if (code == EVENT_CODE_KEY_PRESSED) {
        UInt16 key_code = context.data.u16[0];
        if (key_code == KEY_ESCAPE) {
            event_context data = {};
            event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);
            return TRUE;
        } 
        // else {
        //     KDEBUG("'%c' key pressed in window.", key_code);
        // }
    } 
    // else if (code == EVENT_CODE_KEY_RELEASED) {
    //     UInt16 key_code = context.data.u16[0];
    //     KDEBUG("'%c' key released in window.", key_code);
    // }
    return FALSE;
}

Boolean applicataion_on_resized(UInt16 code, void* sender, void* listener_inst, event_context context) {
    if (code == EVENT_CODE_RESIZED) {
        UInt16 width = context.data.u16[0];
        UInt16 height = context.data.u16[1];

        if (width != app_state->width || height != app_state->height) {
            app_state->width = width;
            app_state->height = height;

            KDEBUG("Window resize: %i, %i", width, height);

            if (width == 0 || height == 0) {
                KINFO("Window minimized, suspending application.");
                app_state->is_suspended = TRUE;
                return TRUE;
            }
            else {
                if (app_state->is_suspended) {
                    KINFO("Window restored, resuming application");
                    app_state->is_suspended = FALSE;
                }

                app_state->game_inst->on_resize(app_state->game_inst, width, height);
                renderer_on_resized(width, height);
            }
        }
    }
    
    return FALSE;
}
