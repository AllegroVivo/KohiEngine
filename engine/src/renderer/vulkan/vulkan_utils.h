#pragma once

#include "vulkan_types.inl"

const char* vulkan_result_string(VkResult result, Boolean get_extended);

Boolean vulkan_result_is_success(VkResult result);