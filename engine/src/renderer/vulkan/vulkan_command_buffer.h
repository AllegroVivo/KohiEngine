#pragma once

#include "vulkan_types.inl"

void vulkan_command_buffer_allocate(
    vulkan_context* context,
    VkCommandPool pool,
    Boolean is_primary,
    vulkan_command_buffer* out_command_buffer);

void vulkan_command_buffer_free(
    vulkan_context* context,
    VkCommandPool pool,
    vulkan_command_buffer* command_buffer);

void vulkan_command_buffer_begin(
    vulkan_command_buffer* command_buffer,
    Boolean is_single_use,
    Boolean is_renderpass_continue,
    Boolean is_simultaneous_use);

void vulkan_command_buffer_end(vulkan_command_buffer* command_buffer);

void vulkan_command_buffer_update_submitted(vulkan_command_buffer* command_buffer);

void vulkan_command_buffer_reset(vulkan_command_buffer* command_buffer);

void vulkan_command_buffer_allocate_and_begin_single_use(
    vulkan_context* context,
    VkCommandPool pool,
    vulkan_command_buffer* out_command_buffer);

void vulkan_command_buffer_end_single_use(
    vulkan_context* context,
    VkCommandPool pool,
    vulkan_command_buffer* command_buffer,
    VkQueue queue);