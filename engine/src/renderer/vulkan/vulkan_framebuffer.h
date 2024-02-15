#pragma once

#include "vulkan_types.inl"

void vulkan_framebuffer_create(
    vulkan_context* context,
    vulkan_renderpass* renderpass,
    UInt32 width,
    UInt32 height,
    UInt32 attachment_count,
    VkImageView* attachments,
    vulkan_framebuffer* out_framebuffer);

void vulkan_framebuffer_destroy(vulkan_context* context, vulkan_framebuffer* framebuffer);