#pragma once

#include "vulkan_types.inl"

Boolean create_shader_module(
    vulkan_context* context,
    const char* name,
    const char* type_str,
    VkShaderStageFlagBits shader_stage_flag,
    UInt32 stage_index,
    vulkan_shader_stage* shader_stages);