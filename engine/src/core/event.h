#pragma once

#include "defines.h"

typedef struct event_context {

    union {
        Int64 i64[2];
        UInt64 u64[2];
        Double f64[2];

        Int32 i32[4];
        UInt32 u32[4];
        Single f32[4];

        Int16 i16[8];
        UInt16 u16[8];

        Int8 i8[16];
        UInt8 u8[16];

        char c[16];
    } data;
} event_context;

typedef Boolean (*PFN_on_event)(UInt16 code, void* sender, void* listener_inst, event_context data);

void event_system_initialize();
void event_system_shutdown();

KAPI Boolean event_register(UInt16 code, void* listener, PFN_on_event on_event);
KAPI Boolean event_unregister(UInt16 code, void* listener, PFN_on_event on_event);
KAPI Boolean event_fire(UInt16 code, void* sender, event_context context);

typedef enum system_event_code {
    EVENT_CODE_APPLICATION_QUIT = 0x01,
    EVENT_CODE_KEY_PRESSED = 0x02,
    EVENT_CODE_KEY_RELEASED = 0x03,
    EVENT_CODE_BUTTON_PRESSED = 0x04,
    EVENT_CODE_BUTTON_RELEASED = 0x05,
    EVENT_CODE_MOUSE_MOVED = 0x06,
    EVENT_CODE_MOUSE_WHEEL = 0x07,
    EVENT_CODE_RESIZED = 0x08,

    MAX_EVENT_CODE = 0xFF
} system_event_code;