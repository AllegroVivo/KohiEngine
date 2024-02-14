#include "containers/darray.h"

#include "core/kmemory.h"
#include "core/logger.h"

void* _darray_create(UInt64 length, UInt64 stride) {
    UInt64 header_size = DARRAY_FIELD_LENGTH * sizeof(UInt64);
    UInt64 array_size = length * stride;
    UInt64* new_array = kallocate(header_size + array_size, MEMORY_TAG_DARRAY);
    kset_memory(new_array, 0, header_size + array_size);
    new_array[DARRAY_CAPACITY] = length;
    new_array[DARRAY_LENGTH] = 0;
    new_array[DARRAY_STRIDE] = stride;
    return (void*)(new_array + DARRAY_FIELD_LENGTH);
}

void _darray_destroy(void* array) {
    UInt64* header = (UInt64*)array - DARRAY_FIELD_LENGTH;
    UInt64 header_size = DARRAY_FIELD_LENGTH * sizeof(UInt64);
    UInt64 total_size = header_size + header[DARRAY_CAPACITY] * header[DARRAY_STRIDE];
    kfree(header, total_size, MEMORY_TAG_DARRAY);
}

UInt64 _darray_field_get(void* array, UInt64 field) {
    UInt64* header = (UInt64*)array - DARRAY_FIELD_LENGTH;
    return header[field];
}

void _darray_field_set(void* array, UInt64 field, UInt64 value) {
    UInt64* header = (UInt64*)array - DARRAY_FIELD_LENGTH;
    header[field] = value;
}

void* _darray_resize(void* array) {
    UInt64 length = darray_length(array);
    UInt64 stride = darray_stride(array);
    void* temp = _darray_create(
        (DARRAY_RESIZE_FACTOR * darray_capacity(array)),
        stride);
    kcopy_memory(temp, array, length * stride);

    _darray_field_set(temp, DARRAY_LENGTH, length);
    _darray_destroy(array);
    return temp;
}

void* _darray_push(void* array, const void* value_ptr) {
    UInt64 length = darray_length(array);
    UInt64 stride = darray_stride(array);
    if (length >= darray_capacity(array)) {
        array = _darray_resize(array);
    }

    UInt64 addr = (UInt64)array;
    addr += (length * stride);
    kcopy_memory((void*)addr, value_ptr, stride);
    _darray_field_set(array, DARRAY_LENGTH, length + 1);
    return array;
}

void _darray_pop(void* array, void* dest) {
    UInt64 length = darray_length(array);
    UInt64 stride = darray_stride(array);
    UInt64 addr = (UInt64)array;
    addr += ((length - 1) * stride);
    kcopy_memory(dest, (void*)addr, stride);
    _darray_field_set(array, DARRAY_LENGTH, length - 1);
}

void* _darray_pop_at(void* array, UInt64 index, void* dest) {
    UInt64 length = darray_length(array);
    UInt64 stride = darray_stride(array);
    if (index >= length) {
        KERROR("Index outside the bounds of this array! Length: %i, index: %index", length, index);
        return array;
    }

    UInt64 addr = (UInt64)array;
    kcopy_memory(dest, (void*)(addr + (index * stride)), stride);

    if (index != length - 1) {
        kcopy_memory(
            (void*)(addr + (index * stride)),
            (void*)(addr + ((index + 1) * stride)),
            stride * (length - index));
    }

    _darray_field_set(array, DARRAY_LENGTH, length - 1);
    return array;
}

void* _darray_insert_at(void* array, UInt64 index, void* value_ptr) {
    UInt64 length = darray_length(array);
    UInt64 stride = darray_stride(array);
    if (index >= length) {
        KERROR("Index outside the bounds of this array! Length: %i, index: %index", length, index);
        return array;
    }
    if (length >= darray_capacity(array)) {
        array = _darray_resize(array);
    }

    UInt64 addr = (UInt64)array;

    if (index != length - 1) {
        kcopy_memory(
            (void*)(addr + ((index + 1) * stride)),
            (void*)(addr + (index * stride)),
            stride * (length - index));
    }

    kcopy_memory((void*)(addr + (index * stride)), value_ptr, stride);

    _darray_field_set(array, DARRAY_LENGTH, length + 1);
    return array;
}