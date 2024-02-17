#pragma once

#include "vulkan_types.inl"

Boolean vulkan_graphics_pipeline_create(
    vulkan_context* context,
    vulkan_renderpass* renderpass,
    UInt32 attribute_count,
    VkVertexInputAttributeDescription* attributes,
    UInt32 descriptor_set_layout_count,
    VkDescriptorSetLayout* descriptor_set_layouts,
    UInt32 stage_count,
    VkPipelineShaderStageCreateInfo* stages,
    VkViewport viewport,
    VkRect2D scissor,
    Boolean is_wireframe,
    vulkan_pipeline* out_pipeline);

void vulkan_pipeline_destroy(vulkan_context* context, vulkan_pipeline* pipeline);

void vulkan_pipeline_bind(vulkan_command_buffer* command_buffer, VkPipelineBindPoint bind_point, vulkan_pipeline* pipeline);