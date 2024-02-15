#pragma once

#include "vulkan_types.inl"

void vulkan_swapchain_create(vulkan_context* context, UInt32 width, UInt32 height, vulkan_swapchain* out_swapchain);
void vulkan_swapchain_recreate(vulkan_context* context, UInt32 width, UInt32 height, vulkan_swapchain* swapchain);

void vulkan_swapchain_destroy(vulkan_context* context, vulkan_swapchain* swapchain);

Boolean vulkan_swapchain_acquire_next_image_index(
    vulkan_context* context,
    vulkan_swapchain* swapchain,
    UInt64 timeout_ms,
    VkSemaphore image_available_semaphore,
    VkFence fence,
    UInt32* out_image_index);

void vulkan_swapchain_present(
    vulkan_context* context,
    vulkan_swapchain* swapchain,
    VkQueue graphics_queue,
    VkQueue present_queue,
    VkSemaphore render_complete_semaphore,
    UInt32 present_image_index);