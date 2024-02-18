#pragma once

#include "vulkan_types.inl"

Boolean vulkan_buffer_create(
    vulkan_context* context,
    UInt64 size,
    VkBufferUsageFlagBits usage,
    UInt32 memory_property_flags,
    Boolean bin_on_create,
    vulkan_buffer* out_buffer);

void vulkan_buffer_destroy(vulkan_context* context, vulkan_buffer* buffer);

Boolean vulkan_buffer_resize(
    vulkan_context* context,
    UInt64 new_size,
    vulkan_buffer* buffer,
    VkQueue queue,
    VkCommandPool pool);

void vulkan_buffer_bind(vulkan_context* context, vulkan_buffer* buffer, UInt64 offset);

void* vulkan_buffer_lock_memory(vulkan_context* context, vulkan_buffer* buffer, UInt64 offset, UInt64 size, UInt32 flags);
void vulkan_buffer_unlock_memory(vulkan_context* context, vulkan_buffer* buffer);

void vulkan_buffer_load_data(vulkan_context* context, vulkan_buffer* buffer, UInt64 offset, UInt64 size, UInt32 flags, const void* data);

void vulkan_buffer_copy_to(
    vulkan_context* context,
    VkCommandPool pool,
    VkFence fence,
    VkQueue queue,
    VkBuffer source,
    UInt64 source_offset,
    VkBuffer dest,
    UInt64 dest_offset,
    UInt64 size);