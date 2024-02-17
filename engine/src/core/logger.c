#include "logger.h"
#include "asserts.h"
#include "platform/platform.h"
#include "platform/filesystem.h"
#include "core/kstring.h"
#include "core/kmemory.h"

#include <stdarg.h>

typedef struct logger_system_state {
    file_handle log_file_handle;
} logger_system_state;

static logger_system_state* state_ptr;

void append_to_log_file(const char* message) {
    if (state_ptr && state_ptr->log_file_handle.is_valid) {
        UInt64 length = string_length(message);
        UInt64 written = 0;
        if (!filesystem_write(&state_ptr->log_file_handle, length, message, &written)) {
            platform_console_write_error("ERROR: Could not write to console.log.", LOG_LEVEL_ERROR);
        }
    }
}

Boolean initialize_logging(UInt64* memory_requirement, void* state) {
    *memory_requirement = sizeof(logger_system_state);

    if (state == 0) {
        return TRUE;
    }

    state_ptr = state;

    if (!filesystem_open("console.log", FILE_MODE_WRITE, FALSE, &state_ptr->log_file_handle)) {
        platform_console_write_error("ERROR: Unable to open console.log for writing.", LOG_LEVEL_ERROR);
        return FALSE;
    }

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
    
    char out_message[32000];
    kzero_memory(out_message, sizeof(out_message));
    
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    string_format_v(out_message, message, arg_ptr);
    va_end(arg_ptr);

    string_format(out_message, "%s%s\n", level_strings[level], out_message);

    if (is_error)
        platform_console_write_error(out_message, level);
    else
        platform_console_write(out_message, level);

    append_to_log_file(out_message);
}
 
void report_assertion_failure(const char* expression, const char* message, const char* file, Int32 line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
}