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

static input_state* state_ptr;

void input_system_initialize(UInt64* memory_requirement, void* state) {
    *memory_requirement = sizeof(input_state);

    if (state == 0) {
        return;
    }

    kzero_memory(state, sizeof(input_state));
    state_ptr = state;
    KINFO("Input subsystem initialized.");
}

void input_system_shutdown(void* state) {
    state_ptr = 0;
}

void input_update(Double delta_time) {
    if (!state_ptr) {
        return;
    }

    kcopy_memory(&state_ptr->keyboard_previous, &state_ptr->keyboard_current, sizeof(keyboard_state));
    kcopy_memory(&state_ptr->mouse_previous, &state_ptr->mouse_current, sizeof(mouse_state));
}

void input_process_key(keys key, Boolean pressed) {
    if (state_ptr && state_ptr->keyboard_current.keys[key] != pressed) {
        state_ptr->keyboard_current.keys[key] = pressed;

        event_context context;
        context.data.u16[0] = key;
        event_fire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
    }
}

void input_process_mouse_button(buttons button, Boolean pressed) {
    if (state_ptr->mouse_current.buttons[button] != pressed) {
        state_ptr->mouse_current.buttons[button] = pressed;

        event_context context;
        context.data.u16[0] = button;
        event_fire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED, 0, context);
    }
}

void input_process_mouse_move(Int16 x, Int16 y) {
    if (state_ptr->mouse_current.x != x || state_ptr->mouse_current.y != y) {
        // KDEBUG("Mouse pos: %i, %i!", x, y);

        state_ptr->mouse_current.x = x;
        state_ptr->mouse_current.y = y;

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
    if (!state_ptr) {
        return FALSE;
    }
    return state_ptr->keyboard_current.keys[key] == TRUE;
}

Boolean input_is_key_up(keys key) {
    if (!state_ptr) {
        return TRUE;
    }
    return state_ptr->keyboard_current.keys[key] == FALSE;
}

Boolean input_was_key_down(keys key) {
    if (!state_ptr) {
        return FALSE;
    }
    return state_ptr->keyboard_previous.keys[key] == TRUE;
}

Boolean input_was_key_up(keys key) {
    if (!state_ptr) {
        return TRUE;
    }
    return state_ptr->keyboard_previous.keys[key] == FALSE;
}

Boolean input_is_button_down(buttons button) {
    if (!state_ptr) {
        return FALSE;
    }
    return state_ptr->mouse_current.buttons[button] == TRUE;
}

Boolean input_is_button_up(buttons button) {
    if (!state_ptr) {
        return TRUE;
    }
    return state_ptr->mouse_current.buttons[button] == FALSE;
}

Boolean input_was_button_down(buttons button) {
    if (!state_ptr) {
        return FALSE;
    }
    return state_ptr->mouse_previous.buttons[button] == TRUE;
}

Boolean input_was_button_up(buttons button) {
    if (!state_ptr) {
        return TRUE;
    }
    return state_ptr->mouse_previous.buttons[button] == FALSE;
}

void input_get_mouse_position(Int32* x, Int32* y) {
    if (!state_ptr) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state_ptr->mouse_current.x;
    *y = state_ptr->mouse_current.y;
}

void input_get_previous_mouse_position(Int32* x, Int32* y) {
    if (!state_ptr) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state_ptr->mouse_previous.x;
    *y = state_ptr->mouse_previous.y;
}