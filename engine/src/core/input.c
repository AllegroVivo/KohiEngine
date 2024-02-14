#include "core/input.h"
#include "core/event.h"
#include "core/kmemory.h"
#include "core/logger.h"

typedef struct keyboard_state {
    Boolean keys[256];
} keyboard_state;

typedef struct mouse_state {
    Int16 x;
    Int16 y;
    UInt8 buttons[BUTTON_MAX_BUTTONS];
} mouse_state;

typedef struct input_state {
    keyboard_state keyboard_current;
    keyboard_state keyboard_previous;
    mouse_state mouse_current;
    mouse_state mouse_previous;
} input_state;

static Boolean initialized = FALSE;
static input_state state = { };

void input_initialize() {
    kzero_memory(&state, sizeof(input_state));
    initialized = TRUE;
    KINFO("Input subsystem initialized.");
}

void input_shutdown() {
    initialized = FALSE;
}

void input_update(Double delta_time) {
    if (!initialized) {
        return;
    }

    kcopy_memory(&state.keyboard_previous, &state.keyboard_current, sizeof(keyboard_state));
    kcopy_memory(&state.mouse_previous, &state.mouse_current, sizeof(mouse_state));
}

void input_process_key(keys key, Boolean pressed) {
    if (state.keyboard_current.keys[key] != pressed) {
        state.keyboard_current.keys[key] = pressed;

        event_context context;
        context.data.u16[0] = key;
        event_fire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
    }
}

void input_process_mouse_button(buttons button, Boolean pressed) {
    if (state.mouse_current.buttons[button] != pressed) {
        state.mouse_current.buttons[button] = pressed;

        event_context context;
        context.data.u16[0] = button;
        event_fire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED, 0, context);
    }
}

void input_process_mouse_move(Int16 x, Int16 y) {
    if (state.mouse_current.x != x || state.mouse_current.y != y) {
        // KDEBUG("Mouse pos: %i, %i!", x, y);

        state.mouse_current.x = x;
        state.mouse_current.y = y;

        event_context context;
        context.data.u16[0] = x;
        context.data.u16[1] = y;
        event_fire(EVENT_CODE_MOUSE_MOVED, 0, context);
    }
}

void input_process_mouse_wheel(Int8 z_delta) {
    event_context context;
    context.data.u8[0] = z_delta;
    event_fire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}

Boolean input_is_key_down(keys key) {
    if (!initialized) {
        return FALSE;
    }
    return state.keyboard_current.keys[key] == TRUE;
}

Boolean input_is_key_up(keys key) {
    if (!initialized) {
        return TRUE;
    }
    return state.keyboard_current.keys[key] == FALSE;
}

Boolean input_was_key_down(keys key) {
    if (!initialized) {
        return FALSE;
    }
    return state.keyboard_previous.keys[key] == TRUE;
}

Boolean input_was_key_up(keys key) {
    if (!initialized) {
        return TRUE;
    }
    return state.keyboard_previous.keys[key] == FALSE;
}

Boolean input_is_button_down(buttons button) {
    if (!initialized) {
        return FALSE;
    }
    return state.mouse_current.buttons[button] == TRUE;
}

Boolean input_is_button_up(buttons button) {
    if (!initialized) {
        return TRUE;
    }
    return state.mouse_current.buttons[button] == FALSE;
}

Boolean input_was_button_down(buttons button) {
    if (!initialized) {
        return FALSE;
    }
    return state.mouse_previous.buttons[button] == TRUE;
}

Boolean input_was_button_up(buttons button) {
    if (!initialized) {
        return TRUE;
    }
    return state.mouse_previous.buttons[button] == FALSE;
}

void input_get_mouse_position(Int32* x, Int32* y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state.mouse_current.x;
    *y = state.mouse_current.y;
}

void input_get_previous_mouse_position(Int32* x, Int32* y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state.mouse_previous.x;
    *y = state.mouse_previous.y;
}