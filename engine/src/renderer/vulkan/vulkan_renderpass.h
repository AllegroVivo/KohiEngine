#pragma once

#include "vulkan_types.inl"

void vulkan_renderpass_create(
    vulkan_context* context,
    vulkan_renderpass* out_renderpass,
    Single x, Single y, Single w, Single h,
    Single r, Single g, Single b, Single a,
    Single depth,
    UInt32 stencil);

void vulkan_renderpass_destroy(vulkan_context* context, vulkan_renderpass* renderpass);

void vulkan_renderpass_begin(
    vulkan_command_buffer* command_buffer,
    vulkan_renderpass* renderpass,
    VkFramebuffer frame_buffer);

void vulkan_renderpass_end(vulkan_command_buffer* command_buffer, vulkan_renderpass* renderpass);