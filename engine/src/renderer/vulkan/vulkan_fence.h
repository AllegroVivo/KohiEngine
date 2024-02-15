#pragma once

#include "vulkan_types.inl"

void vulkan_fence_create(vulkan_context* context, Boolean create_signaled, vulkan_fence* out_fence);
void vulkan_fence_destroy(vulkan_context* context, vulkan_fence* fence);

Boolean vulkan_fence_wait(vulkan_context* context, vulkan_fence* fence, UInt64 timeout_ns);

void vulkan_fence_reset(vulkan_context* context, vulkan_fence* fence);