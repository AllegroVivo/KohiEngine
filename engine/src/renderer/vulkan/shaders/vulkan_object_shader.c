#include "vulkan_object_shader.h"
#include "core/logger.h"
#include "renderer/vulkan/vulkan_shader_utils.h"

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

    return TRUE;
}

void vulkan_object_shader_destroy(vulkan_context* context, struct vulkan_object_shader* shader) {

}

void vulkan_object_shader_user(vulkan_context* context, struct vulkan_object_shader* shader) {

}