#pragma once

#include "defines.h"

typedef struct file_handle {
    void* handle;
    Boolean is_valid;
} file_handle;

typedef enum  file_modes { 
    FILE_MODE_READ = 0x1,
    FILE_MODE_WRITE = 0x2
} file_modes;

KAPI Boolean filesystem_exists(const char* path);

KAPI Boolean filesystem_open(const char* path, file_modes mode, Boolean binary, file_handle* out_handle);

KAPI void filesystem_close(file_handle* handle);

KAPI Boolean filesystem_read_line(file_handle* handle, char** line_buff);

KAPI Boolean filesystem_write_line(file_handle* handle, const char* text);

KAPI Boolean filesystem_read(file_handle* handle, UInt64 data_size, void* out_data, UInt64* out_bytes_read);

KAPI Boolean filesystem_read_all_bytes(file_handle* handle, UInt8** out_bytes, UInt64* out_bytes_read);

KAPI Boolean filesystem_write(file_handle* handle, UInt64 data_size, const void* data, UInt64* out_bytes_written);