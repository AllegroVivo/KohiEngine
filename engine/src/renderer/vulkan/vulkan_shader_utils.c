#include "vulkan_shader_utils.h"

#include "core/logger.h"
#include "core/kstring.h"
#include "core/kmemory.h"

#include "platform/filesystem.h"

Boolean create_shader_module(
    vulkan_context* context,
    const char* name,
    const char* type_str,
    VkShaderStageFlagBits shader_stage_flag,
    UInt32 stage_index,
    vulkan_shader_stage* shader_stages) {

    char file_name[512];
    string_format(file_name, "assets/shaders/%s.%s.spv", name, type_str);

    kzero_memory(&shader_stages[stage_index].create_info, sizeof(VkShaderModuleCreateInfo));
    shader_stages[stage_index].create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    file_handle handle;
    if (!filesystem_open(file_name, FILE_MODE_READ, TRUE, &handle)) {
        KERROR("Unable to read shader module: %s", file_name);
        return FALSE;
    }

    UInt64 size = 0;
    UInt8* file_buffer = 0;
    if (!filesystem_read_all_bytes(&handle, &file_buffer, &size)) {
        KERROR("Unable to read binary shader module: %s", file_name);
        return FALSE;
    }

    shader_stages[stage_index].create_info.codeSize = size;
    shader_stages[stage_index].create_info.pCode = (UInt32*)file_buffer;

    filesystem_close(&handle);

    VK_CHECK(vkCreateShaderModule(
        context->device.logical_device,
        &shader_stages[stage_index].create_info,
        context->allocator,
        &shader_stages[stage_index].handle));

    kzero_memory(&shader_stages[stage_index].shader_state_create_info, sizeof(VkPipelineShaderStageCreateInfo));
    shader_stages[stage_index].shader_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[stage_index].shader_state_create_info.stage = shader_stage_flag;
    shader_stages[stage_index].shader_state_create_info.module = shader_stages[stage_index].handle;
    shader_stages[stage_index].shader_state_create_info.pName = "main";

    if (file_buffer) {
        kfree(file_buffer, sizeof(UInt8) * size, MEMORY_TAG_STRING);
        file_buffer = 0;
    }

    return TRUE;
}