#include "vulkan_backend.h"

#include "vulkan_types.inl"
#include "vulkan_platform.h"
#include "vulkan_device.h"
#include "vulkan_swapchain.h"
#include "vulkan_renderpass.h"
#include "vulkan_command_buffer.h"
#include "vulkan_framebuffer.h"
#include "vulkan_fence.h"
#include "vulkan_utils.h"
#include "vulkan_buffer.h"

#include "core/application.h"
#include "core/logger.h"
#include "core/kstring.h"
#include "core/kmemory.h"

#include "containers/darray.h"

#include "math/math_types.h"

#include "platform/platform.h"

#include "shaders/vulkan_object_shader.h"

static vulkan_context context;
static UInt32 cached_framebuffer_width = 0;
static UInt32 cached_framebuffer_height = 0;

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data);

Int32 find_memory_index(UInt32 type_filter, UInt32 property_flags);
Boolean create_buffers(vulkan_context* context);

void create_command_buffers(renderer_backend* backend);
void regenerate_framebuffers(renderer_backend* backend, vulkan_swapchain* swapchain, vulkan_renderpass* renderpass);
Boolean recreate_swapchain(renderer_backend* backend);

void upload_data_range(vulkan_context* context, VkCommandPool pool, VkFence fence, VkQueue queue, vulkan_buffer* buffer, UInt64 offset, UInt64 size, void* data) {
    VkBufferUsageFlags flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    vulkan_buffer staging;
    vulkan_buffer_create(context, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, flags, TRUE, &staging);

    vulkan_buffer_load_data(context, &staging, 0, size, 0, data);

    vulkan_buffer_copy_to(context, pool, fence, queue, staging.handle, 0, buffer->handle, offset, size);

    vulkan_buffer_destroy(context, &staging);
}

Boolean vulkan_renderer_backend_initialize(renderer_backend* backend, const char* application_name) {

    context.find_memory_index = find_memory_index;
    context.allocator = 0;

    application_get_framebuffer_size(&cached_framebuffer_width, &cached_framebuffer_height);
    context.framebuffer_width = cached_framebuffer_width != 0 ? cached_framebuffer_width : 800;
    context.framebuffer_height = cached_framebuffer_height != 0 ? cached_framebuffer_height : 600;
    cached_framebuffer_width = 0;
    cached_framebuffer_height = 0;

    VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pApplicationName = application_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Kohi Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    create_info.pApplicationInfo = &app_info;

    const char** required_extensions = darray_create(const char*);
    darray_push(required_extensions, &VK_KHR_SURFACE_EXTENSION_NAME);
    platform_get_required_extension_names(&required_extensions);
#if defined(_DEBUG)
    darray_push(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    KDEBUG("Required extensions:");
    UInt32 length = darray_length(required_extensions);
    for (UInt32 i = 0; i< length; ++i) {
        KDEBUG(required_extensions[i]);
    }
#endif

    create_info.enabledExtensionCount = darray_length(required_extensions);
    create_info.ppEnabledExtensionNames = required_extensions;

    const char** required_validation_layer_names = 0;
    UInt32 required_validation_layer_count = 0;

#if defined(_DEBUG)
    KINFO("Validation layers enabled. Enumerating...");

    required_validation_layer_names = darray_create(const char*);
    darray_push(required_validation_layer_names, &"VK_LAYER_KHRONOS_validation");
    required_validation_layer_count = darray_length(required_validation_layer_names);

    UInt32 available_layer_count = 1;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, 0));
    VkLayerProperties* available_layers = darray_reserve(VkLayerProperties, available_layer_count);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers));

    for (UInt32 i = 0; i < required_validation_layer_count; ++i) {
        KINFO("Searching for later: %s...", required_validation_layer_names[i]);
        Boolean found = FALSE;
        for (UInt32 j = 0; j < available_layer_count; ++j) {
            if (strings_equal(required_validation_layer_names[i], available_layers[j].layerName)) {
                found = TRUE;
                KINFO("Found.");
                break;
            }
        }

        if (!found) {
            KFATAL("Required validation layer is missing: %s", required_validation_layer_names[i]);
            return FALSE;
        }
    }

    KINFO("All required validation layers are present.");
#endif

    create_info.enabledLayerCount = required_validation_layer_count;
    create_info.ppEnabledLayerNames = required_validation_layer_names;

    VK_CHECK(vkCreateInstance(&create_info, context.allocator, &context.instance));
    KINFO("Vulkan instance created.");

#if defined(_DEBUG)
    KDEBUG("Creating Vulkan debugger...");
    UInt32 log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
    debug_create_info.messageSeverity = log_severity;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_create_info.pfnUserCallback = vk_debug_callback;

    PFN_vkCreateDebugUtilsMessengerEXT func = 
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
    KASSERT_MSG(func, "Failed to create debug messenger!");
    VK_CHECK(func(context.instance, &debug_create_info, context.allocator, &context.debug_messenger));
    KDEBUG("Vulkan debugger created~!");
#endif

    KDEBUG("Creating Vulkan surface...");
    if (!platform_create_vulkan_surface(&context)) {
        KERROR("Failed to create platform surface!");
        return FALSE;
    }

    KDEBUG("Vulkan surface created.");

    // Device creation
    if (!vulkan_device_create(&context)) {
        KERROR("Failed to create device!");
        return FALSE;
    }

    vulkan_swapchain_create(
        &context,
        context.framebuffer_width,
        context.framebuffer_height,
        &context.swapchain);

    vulkan_renderpass_create(
        &context,
        &context.main_renderpass,
        0, 0, context.framebuffer_width, context.framebuffer_height,
        0.f, 0.f, 0.2f, 1.f,
        1.f, 0);

    context.swapchain.framebuffers = darray_reserve(vulkan_framebuffer, context.swapchain.image_count);
    regenerate_framebuffers(backend, &context.swapchain, &context.main_renderpass);

    create_command_buffers(backend);

    context.image_available_semaphores = darray_reserve(VkSemaphore, context.swapchain.max_frames_in_flight);
    context.queue_complete_semaphores = darray_reserve(VkSemaphore, context.swapchain.max_frames_in_flight);
    context.in_flight_fences = darray_reserve(vulkan_fence, context.swapchain.max_frames_in_flight);

    for (UInt8 i = 0; i < context.swapchain.max_frames_in_flight; ++i) {
        VkSemaphoreCreateInfo semaphore_create_info = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
        vkCreateSemaphore(context.device.logical_device, &semaphore_create_info, context.allocator, &context.image_available_semaphores[i]);
        vkCreateSemaphore(context.device.logical_device, &semaphore_create_info, context.allocator, &context.queue_complete_semaphores[i]);

        vulkan_fence_create(&context, TRUE, &context.in_flight_fences[i]);
    }
    
    context.images_in_flight = darray_reserve(vulkan_fence, context.swapchain.image_count);
    for (UInt32 i = 0; i < context.swapchain.image_count; ++i) {
        context.images_in_flight[i] = 0;
    }

    if (!vulkan_object_shader_create(&context, &context.object_shader)) {
        KERROR("Error loading built-in basic_lighting shader");
        return FALSE;
    }

    create_buffers(&context);

    const UInt32 vert_count = 4;
    vertex_3d verts[vert_count];
    kzero_memory(verts, sizeof(vertex_3d) * vert_count);

    const Single f = 10.0f;

    verts[0].position.x = -0.5 * f;
    verts[0].position.y = -0.5 * f;

    verts[1].position.x = 0.5 * f;
    verts[1].position.y = 0.5 * f;

    verts[2].position.x = -0.5 * f;
    verts[2].position.y = 0.5 * f;

    verts[3].position.x = 0.5 * f;
    verts[3].position.y = -0.5 * f;

    const UInt32 index_count = 6;
    UInt32 indices[index_count] = { 0, 1, 2, 0, 3, 1 };

    upload_data_range(&context, context.device.graphics_command_pool, 0, context.device.graphics_queue, &context.object_vertex_buffer, 0, sizeof(vertex_3d) * vert_count, verts);
    upload_data_range(&context, context.device.graphics_command_pool, 0, context.device.graphics_queue, &context.object_index_buffer, 0, sizeof(UInt32) * index_count, indices);

    KINFO("Vulkan renderer intialized successfully.");
    return TRUE;
}

void vulkan_renderer_backend_shutdown(renderer_backend* backend) {

    vkDeviceWaitIdle(context.device.logical_device);

    vulkan_buffer_destroy(&context, &context.object_vertex_buffer);
    vulkan_buffer_destroy(&context, &context.object_index_buffer);


    vulkan_object_shader_destroy(&context, &context.object_shader);

    for (UInt8 i = 0; i < context.swapchain.max_frames_in_flight; ++i) {
        if (context.image_available_semaphores[i]) {
            vkDestroySemaphore(
                context.device.logical_device,
                context.image_available_semaphores[i],
                context.allocator);
            context.image_available_semaphores[i] = 0;
        }
        if (context.queue_complete_semaphores[i]) {
            vkDestroySemaphore(
                context.device.logical_device,
                context.queue_complete_semaphores[i],
                context.allocator);
            context.queue_complete_semaphores[i] = 0;
        }

        vulkan_fence_destroy(&context, &context.in_flight_fences[i]);
    }

    darray_destroy(context.image_available_semaphores);
    context.image_available_semaphores = 0;
    
    darray_destroy(context.queue_complete_semaphores);
    context.queue_complete_semaphores = 0;

    darray_destroy(context.in_flight_fences);
    context.in_flight_fences = 0;

    darray_destroy(context.images_in_flight);
    context.images_in_flight = 0;
        
    for (UInt32 i = 0; i < context.swapchain.image_count; i++) {
        if (context.graphics_command_buffers[i].handle) {
            vulkan_command_buffer_free(&context, context.device.graphics_command_pool, &context.graphics_command_buffers[i]);
            context.graphics_command_buffers[i].handle = 0;
        }
    }
    darray_destroy(context.graphics_command_buffers);
    context.graphics_command_buffers = 0;

    for (UInt32 i = 0; i < context.swapchain.image_count; ++i) {
        vulkan_framebuffer_destroy(&context, &context.swapchain.framebuffers[i]);
    }
    

    vulkan_renderpass_destroy(&context, &context.main_renderpass);
    vulkan_swapchain_destroy(&context, &context.swapchain);
    
    KDEBUG("Destroying Vulkan device...");
    vulkan_device_destroy(&context);

    KDEBUG("Destroying Vulkan surface...");
    if (context.surface) {
        vkDestroySurfaceKHR(context.instance, context.surface, context.allocator);
        context.surface = 0;
    }

    KDEBUG("Destroying Vulkan debugger");
    if(context.debug_messenger) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = 
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
        func(context.instance, context.debug_messenger, context.allocator);
    }

    KDEBUG("Destroying Vulkan instance...");
    vkDestroyInstance(context.instance, context.allocator);
}

void vulkan_renderer_backend_on_resized(renderer_backend* backend, UInt16 width, UInt16 height) {
    cached_framebuffer_width = width;
    cached_framebuffer_height = height;
    context.framebuffer_size_generation++;

    KINFO("Vulkan renderer backend->resized: w/h/gen: %i/%i/%llu", width, height, context.framebuffer_size_generation);
}

Boolean vulkan_renderer_backend_begin_frame(renderer_backend* backend, Single delta_time) {
    vulkan_device* device = &context.device;

    if (context.recreating_swapchain) {
        VkResult result = vkDeviceWaitIdle(device->logical_device);
        if (!vulkan_result_is_success(result)) {
            KERROR("vulkan_renderer_backend_begin_frame vkDeviceWaitIdle (1) failed: '%s'", vulkan_result_string(result, TRUE));
            return FALSE;
        }

        KINFO("Recreating swapchain, booting...");
        return FALSE;
    }

    if (context.framebuffer_size_generation != context.framebuffer_size_last_generation) {
        VkResult result = vkDeviceWaitIdle(device->logical_device);
        if (!vulkan_result_is_success(result)) {
            KERROR("vulkan_renderer_backend_begin_frame vkDeviceWaitIdle (2) failed: '%s'", vulkan_result_string(result, TRUE));
            return FALSE;
        }

        if (!recreate_swapchain(backend)) {
            return FALSE;
        }

        KINFO("Resized. Booting...");
        return FALSE;
    }

    if (!vulkan_fence_wait(&context, &context.in_flight_fences[context.current_frame], UINT64_MAX)) {
        KWARN("In-flight fence wait failure.");
        return FALSE;
    }

    if (!vulkan_swapchain_acquire_next_image_index(
            &context,
            &context.swapchain,
            UINT64_MAX,
            context.image_available_semaphores[context.current_frame],
            0,
            &context.image_index)) {
        return FALSE;
        }

    vulkan_command_buffer* command_buffer = &context.graphics_command_buffers[context.image_index];
    vulkan_command_buffer_reset(command_buffer);
    vulkan_command_buffer_begin(command_buffer, FALSE, FALSE, FALSE);

    VkViewport viewport;
    viewport.x = 0.f;
    viewport.y = (Single)context.framebuffer_height;
    viewport.width = (Single)context.framebuffer_width;
    viewport.height = -(Single)context.framebuffer_height;
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;

    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = context.framebuffer_width;
    scissor.extent.height = context.framebuffer_height;

    vkCmdSetViewport(command_buffer->handle, 0, 1, &viewport);
    vkCmdSetScissor(command_buffer->handle, 0, 1, &scissor);

    context.main_renderpass.w = context.framebuffer_width;
    context.main_renderpass.h = context.framebuffer_height;

    vulkan_renderpass_begin(command_buffer, &context.main_renderpass, context.swapchain.framebuffers[context.image_index].handle);

    return TRUE;
}

void vulkan_renderer_update_global_state(mat4 projection, mat4 view, vec3 view_position, vec4 ambient_colour, Int32 mode) {
    vulkan_command_buffer* command_buffer = &context.graphics_command_buffers[context.image_index];

    vulkan_object_shader_use(&context, &context.object_shader);

    context.object_shader.global_ubo.projection = projection;
    context.object_shader.global_ubo.view = view;

    vulkan_object_shader_update_global_state(&context, &context.object_shader);

    vulkan_object_shader_use(&context, &context.object_shader);

    VkDeviceSize offsets[1] = {0};
    vkCmdBindVertexBuffers(command_buffer->handle, 0, 1, &context.object_vertex_buffer.handle, (VkDeviceSize*)offsets);

    vkCmdBindIndexBuffer(command_buffer->handle, context.object_index_buffer.handle, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(command_buffer->handle, 6, 1, 0, 0, 0);
}

Boolean vulkan_renderer_backend_end_frame(renderer_backend* backend, Single delta_time) {
    vulkan_command_buffer* command_buffer = &context.graphics_command_buffers[context.image_index];

    vulkan_renderpass_end(command_buffer, &context.main_renderpass);
    vulkan_command_buffer_end(command_buffer);

    if (context.images_in_flight[context.image_index] != VK_NULL_HANDLE) {
        vulkan_fence_wait(&context, context.images_in_flight[context.image_index], UINT64_MAX);
    }

    context.images_in_flight[context.image_index] = &context.in_flight_fences[context.current_frame];
    vulkan_fence_reset(&context, &context.in_flight_fences[context.current_frame]);

    VkSubmitInfo submit_info = { VK_STRUCTURE_TYPE_SUBMIT_INFO };

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = & command_buffer->handle;

    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &context.queue_complete_semaphores[context.current_frame];

    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &context.image_available_semaphores[context.current_frame];

    VkPipelineStageFlags flags[1] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.pWaitDstStageMask = flags;

    VkResult result = vkQueueSubmit(
        context.device.graphics_queue,
        1,
        &submit_info,
        context.in_flight_fences[context.current_frame].handle);
    if (result != VK_SUCCESS) {
        KERROR("vkQueueSubmit failed with result: %s", vulkan_result_string(result, TRUE));
    }

    vulkan_command_buffer_update_submitted(command_buffer);

    vulkan_swapchain_present(
        &context,
        &context.swapchain,
        context.device.graphics_queue,
        context.device.present_queue,
        context.queue_complete_semaphores[context.current_frame],
        context.image_index);

    return TRUE;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data) {
    switch (message_severity) {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            KERROR(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            KWARN(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            KINFO(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            KTRACE(callback_data->pMessage);
            break;
    }

    return VK_FALSE;
}

Int32 find_memory_index(UInt32 type_filter, UInt32 property_flags) {
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(context.device.physical_device, &memory_properties);

    for (UInt32 i = 0; i < memory_properties.memoryTypeCount; ++i) {
        if (type_filter & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags) {
            return i;
        }
    }
    
    KWARN("Unable to find suitable memory type!");
    return -1;
}

void create_command_buffers(renderer_backend* backend) {
    if (!context.graphics_command_buffers) {
        context.graphics_command_buffers = darray_reserve(vulkan_command_buffer, context.swapchain.image_count);
        for (UInt32 i = 0; i < context.swapchain.image_count; ++i) {
            kzero_memory(&context.graphics_command_buffers[i], sizeof(vulkan_command_buffer));
        }
    }

    for (UInt32 i = 0; i < context.swapchain.image_count; ++i) {
        if (context.graphics_command_buffers[i].handle) {
            vulkan_command_buffer_free(
                &context,
                context.device.graphics_command_pool,
                &context.graphics_command_buffers[i]);
        }
        kzero_memory(&context.graphics_command_buffers[i], sizeof(vulkan_command_buffer));
        vulkan_command_buffer_allocate(
            &context,
            context.device.graphics_command_pool,
            TRUE,
            &context.graphics_command_buffers[i]);
    }
    
    KINFO("Vulkan command buffers created.");
}

void regenerate_framebuffers(renderer_backend* backend, vulkan_swapchain* swapchain, vulkan_renderpass* renderpass) {
    for (UInt32 i = 0; i < swapchain->image_count; ++i) {
        UInt32 attachment_count = 2;
        VkImageView attachments[] = {
            swapchain->views[i],
            swapchain->depth_attachment.view};
        
        vulkan_framebuffer_create(
            &context,
            renderpass,
            context.framebuffer_width,
            context.framebuffer_height,
            attachment_count,
            attachments,
            &context.swapchain.framebuffers[i]);
    }
}

Boolean recreate_swapchain(renderer_backend* backend) {

    if (context.recreating_swapchain) {
        KDEBUG("recreate_swapchain called when already recreating. Booting.");
        return FALSE;
    }

    if (context.framebuffer_width == 0 || context.framebuffer_height == 0) {
        KDEBUG("recreate_swapchain called when window is < 1 in a dimension. Booting.");
        return FALSE;
    }

    context.recreating_swapchain = TRUE;

    vkDeviceWaitIdle(context.device.logical_device);

    for (UInt32 i = 0; i < context.swapchain.image_count; ++i) {
        context.images_in_flight[i] = 0;
    }

    vulkan_device_query_swapchain_support(
        context.device.physical_device,
        context.surface,
        &context.device.swapchain_support);
    vulkan_device_detect_depth_format(&context.device);

    vulkan_swapchain_recreate(
        &context,
        cached_framebuffer_width,
        cached_framebuffer_height,
        &context.swapchain);

    context.framebuffer_width = cached_framebuffer_width;
    context.framebuffer_height = cached_framebuffer_height;
    context.main_renderpass.w = context.framebuffer_width;
    context.main_renderpass.h = context.framebuffer_height;
    cached_framebuffer_width = 0;
    cached_framebuffer_height = 0;

    context.framebuffer_size_last_generation = context.framebuffer_size_generation;

    for (UInt32 i = 0; i < context.swapchain.image_count; ++i) {
        vulkan_command_buffer_free(&context, context.device.graphics_command_pool, &context.graphics_command_buffers[i]);
    }

    for (UInt32 i = 0; i < context.swapchain.image_count; ++i) {
        vulkan_framebuffer_destroy(&context, &context.swapchain.framebuffers[i]);
    }

    context.main_renderpass.x = 0;
    context.main_renderpass.y = 0;
    context.main_renderpass.w = context.framebuffer_width;
    context.main_renderpass.h = context.framebuffer_height;

    regenerate_framebuffers(backend, &context.swapchain, &context.main_renderpass);
    create_command_buffers(backend);

    context.recreating_swapchain = FALSE;

    return TRUE;
}

Boolean create_buffers(vulkan_context* context) {
    VkMemoryPropertyFlagBits memory_property_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    const UInt64 vertex_buffer_size = sizeof(vertex_3d) * 1024 * 1024;
    if (!vulkan_buffer_create(
            context,
            vertex_buffer_size,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            memory_property_flags,
            TRUE,
            &context->object_vertex_buffer)) {
        KERROR("Error creating vertex buffer.");
        return FALSE;
    }
    context->geometry_vertex_offset = 0;

    const UInt64 index_buffer_size = sizeof(UInt32) * 1024 * 1024;
    if (!vulkan_buffer_create(
            context,
            index_buffer_size,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            memory_property_flags,
            TRUE,
            &context->object_index_buffer)) {
        KERROR("Error creating vertex buffer.");
        return FALSE;
    }
    context->geometry_index_offset = 0;

    return TRUE;
}