#include "logger.h"
#include "asserts.h"
#include "platform/platform.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

typedef struct logger_system_state {
    Boolean initialized;
} logger_system_state;

static logger_system_state* state_ptr;

Boolean initialize_logging(UInt64* memory_requirement, void* state) {
    *memory_requirement = sizeof(logger_system_state);

    if (state == 0) {
        return TRUE;
    }

    state_ptr = state;
    state_ptr->initialized = TRUE;

    // KFATAL("Test fatal message");
    // KERROR("Test error message");
    // KWARN("Test warning message");
    // KINFO("Test info message");
    // KDEBUG("Test debug message");
    // KTRACE("Test trace message");

    return TRUE;
}

void shutdown_logging(void* state) {
    state_ptr = 0;
}

void log_output(log_level level, const char* message, ...) {
    const char* level_strings[6] = { "[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };
    Boolean is_error = level < LOG_LEVEL_WARN;
    
    const Int32 msg_length = 32000;
    char out_message[msg_length];
    memset(out_message, 0, sizeof(out_message));
    
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, msg_length, message, arg_ptr);
    va_end(arg_ptr);

    char out_message2[msg_length];
    sprintf(out_message2, "%s%s\n", level_strings[level], out_message);

    if (is_error)
        platform_console_write_error(out_message2, level);
    else
        platform_console_write(out_message2, level);

}
 
void report_assertion_failure(const char* expression, const char* message, const char* file, Int32 line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
}