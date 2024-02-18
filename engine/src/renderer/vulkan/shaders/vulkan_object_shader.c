#include "vulkan_object_shader.h"
#include "core/logger.h"
#include "renderer/vulkan/vulkan_shader_utils.h"
#include "math/math_types.h"
#include "renderer/vulkan/vulkan_pipeline.h"
#include "core/kmemory.h"

#define BUILTIN_SHADER_NAME_OBJECT "Builtin.ObjectShader"

Boolean vulkan_object_shader_create(vulkan_context* context, vulkan_object_shader* out_shader) {
    char stage_type_strs[OBJECT_SHADER_STAGE_COUNT][5] = { "vert", "frag" };
    VkShaderStageFlagBits stage_types[OBJECT_SHADER_STAGE_COUNT] = { VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT };

    for (UInt32 i = 0; i < OBJECT_SHADER_STAGE_COUNT; ++i) {
        if (!create_shader_module(context, BUILTIN_SHADER_NAME_OBJECT, stage_type_strs[i], stage_types[i], i, out_shader->stages)) {
            KERROR("Unable to create %s shader moldule for '%s'.", stage_type_strs[i], BUILTIN_SHADER_NAME_OBJECT);
            return FALSE;
        }
    }

    VkViewport viewport;
    viewport.x = 0.f;
    viewport.y = (Single)context->framebuffer_height;
    viewport.width = (Single)context->framebuffer_width;
    viewport.height = (Single)context->framebuffer_height;
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;

    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = context->framebuffer_width;
    scissor.extent.height = context->framebuffer_height;

    UInt32 offset = 0;
    const Int32 attribute_count = 1;
    VkVertexInputAttributeDescription attribute_descriptions[attribute_count];

    VkFormat formats[attribute_count] = {
        VK_FORMAT_R32G32B32_SFLOAT
    };
    UInt64 sizes[attribute_count] = {
        sizeof(vec3)
    };

    for (UInt32 i = 0; i < attribute_count; ++i) {
        attribute_descriptions[i].binding = 0;
        attribute_descriptions[i].location = i;
        attribute_descriptions[i].format = formats[i];
        attribute_descriptions[i].offset = offset;
        offset += sizes[i];
    }

    VkPipelineShaderStageCreateInfo stage_create_infos[OBJECT_SHADER_STAGE_COUNT];
    kzero_memory(stage_create_infos, sizeof(stage_create_infos));
    for (UInt32 i = 0; i < OBJECT_SHADER_STAGE_COUNT; ++i) {
        stage_create_infos[i].sType = out_shader->stages[i].shader_state_create_info.sType;
        stage_create_infos[i] = out_shader->stages[i].shader_state_create_info;
    }

    if (!vulkan_graphics_pipeline_create(
            context,
            &context->main_renderpass,
            attribute_count,
            attribute_descriptions,
            0,
            0,
            OBJECT_SHADER_STAGE_COUNT,
            stage_create_infos,
            viewport,
            scissor,
            FALSE,
            &out_shader->pipeline)) {
        KERROR("Failed to load graphics pipeline for object shader.");
        return FALSE;
    }
    

    return TRUE;
}

void vulkan_object_shader_destroy(vulkan_context* context, struct vulkan_object_shader* shader) {

    vulkan_pipeline_destroy(context, &shader->pipeline);
    for (UInt32 i = 0; i < OBJECT_SHADER_STAGE_COUNT; ++i) {
        vkDestroyShaderModule(context->device.logical_device, shader->stages[i].handle, context->allocator);
        shader->stages[i].handle = 0;
    }
    
}

void vulkan_object_shader_use(vulkan_context* context, struct vulkan_object_shader* shader) {
    UInt32 image_index = context->image_index;
    vulkan_pipeline_bind(&context->graphics_command_buffers[image_index], VK_PIPELINE_BIND_POINT_GRAPHICS, &shader->pipeline);
}