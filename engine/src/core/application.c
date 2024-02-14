#include "application.h"
#include "logger.h"
#include "platform/platform.h"
#include "game_types.h"
#include "core/kmemory.h"
#include "core/event.h"
#include "core/input.h"

typedef struct application_state
{
    game* game_inst;
    Boolean is_running;
    Boolean is_suspended;
    platform_state platform;
    Int16 width;
    Int16 height;
    Double last_time;
} application_state;

static Boolean initialized = FALSE;
static application_state app_state;

Boolean application_on_event(UInt16 code, void* sender, void* listener_inst, event_context context);
Boolean application_on_key(UInt16 code, void* sender, void* listener_inst, event_context context);

Boolean application_create(game* game_inst) {
    if (initialized) {
        KERROR("application_create called more than once.");
        return FALSE;
    }

    app_state.game_inst = game_inst;
    
    initialize_logging();
    input_initialize();
    
    KFATAL("Test fatal message");
    KERROR("Test error message");
    KWARN("Test warning message");
    KINFO("Test info message");
    KDEBUG("Test debug message");
    KTRACE("Test trace message");
    
    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if (!event_initialize()) {
        KERROR("Event system failed initialization. Application cannot continue.");
        return FALSE;
    }

    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);
    
    if (!platform_startup(
            &app_state.platform, 
            game_inst->app_config.name, 
            game_inst->app_config.start_pos_x,
            game_inst->app_config.start_pos_y,
            game_inst->app_config.start_width, 
            game_inst->app_config.start_height)) {
        return FALSE;
    }

    if (!app_state.game_inst->initialize(app_state.game_inst)) {
        KFATAL("Game failed to initialize");
        return FALSE;
    }

    app_state.game_inst->on_resize(app_state.game_inst, app_state.width, app_state.height);

    initialized = TRUE;

    return TRUE;
}

Boolean application_run() {

    KINFO(get_memory_usage_str());

    while (app_state.is_running) {
        if (!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            if (!app_state.game_inst->update(app_state.game_inst, (Single)0)) {
                KFATAL("Game update failed, shutting down.");
                app_state.is_running = FALSE;
                break;
            }

            if (!app_state.game_inst->render(app_state.game_inst, (Single)0)) {
                KFATAL("Game render failed, shutting down.");
                app_state.is_running = FALSE;
                break;
            }

            input_update(0);
        }
    }

    app_state.is_running = FALSE;

    event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_unregister(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

    event_shutdown();
    input_shutdown();

    platform_shutdown(&app_state.platform);

    return TRUE;
}

Boolean application_on_event(UInt16 code, void* sender, void* listener_inst, event_context context) {
    switch (code) {
        case EVENT_CODE_APPLICATION_QUIT: {
            KINFO("EVENT_CODE_APPLICATION_QUIT recieved, shutting down.\n");
            app_state.is_running = FALSE;
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
        else {
            KDEBUG("'%c' key pressed in window.", key_code);
        }
    } 
    else if (code == EVENT_CODE_KEY_RELEASED) {
        UInt16 key_code = context.data.u16[0];
        KDEBUG("'%c' key released in window.", key_code);
    }
    return FALSE;
}