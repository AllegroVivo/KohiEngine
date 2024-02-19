#pragma once

#include "defines.h"
#include "core/asserts.h"
#include "renderer/renderer_types.inl"

#include <vulkan/vulkan.h>

#define VK_CHECK(expr)               \
    {                                \
        KASSERT(expr == VK_SUCCESS); \
    }

typedef struct vulkan_buffer {
    UInt64 total_size;
    VkBuffer handle;
    VkBufferUsageFlagBits usage;
    Boolean is_locked;
    VkDeviceMemory memory;
    Int32 memory_index;
    UInt32 memory_property_flags;
} vulkan_buffer;


typedef struct vulkan_swapchain_support_info {
    VkSurfaceCapabilitiesKHR capabilities;
    UInt32 format_count;
    VkSurfaceFormatKHR* formats;
    UInt32 present_mode_count;
    VkPresentModeKHR* present_modes;
} vulkan_swapchain_support_info;

typedef struct vulkan_device {
    VkPhysicalDevice physical_device;
    VkDevice logical_device;
    vulkan_swapchain_support_info swapchain_support;
    Int32 graphics_queue_index;
    Int32 present_queue_index;
    Int32 transfer_queue_index;

    VkQueue graphics_queue;
    VkQueue present_queue;
    VkQueue transfer_queue;

    VkCommandPool graphics_command_pool;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memory;

    VkFormat depth_format;
} vulkan_device;

typedef struct vulkan_image {
    VkImage handle;
    VkDeviceMemory memory;
    VkImageView view;
    UInt32 width;
    UInt32 height;
} vulkan_image;

typedef enum vulkan_render_pass_state {
    READY, 
    RECORDING,
    IN_RENDER_PASS,
    RECORDING_ENDED,
    SUBMITTED,
    NOT_ALLOCATED
} vulkan_render_pass_state;

typedef struct vulkan_renderpass {
    VkRenderPass handle;
    Single x, y, w, h;
    Single r, g, b, a;

    Single depth;
    UInt32 stencil;

    vulkan_render_pass_state state;
} vulkan_renderpass;

typedef struct vulkan_framebuffer {
    VkFramebuffer handle;
    UInt32 attachment_count;
    VkImageView* attachments;
    vulkan_renderpass* renderpass;
} vulkan_framebuffer;

typedef struct vulkan_swapchain {
    VkSurfaceFormatKHR image_format;
    UInt8 max_frames_in_flight;
    VkSwapchainKHR handle;
    UInt32 image_count;
    VkImage* images;
    VkImageView* views;

    vulkan_image depth_attachment;

    vulkan_framebuffer* framebuffers;
} vulkan_swapchain; 

typedef enum vulkan_command_buffer_state {
    COMMAND_BUFFER_STATE_READY,
    COMMAND_BUFFER_STATE_RECORDING,
    COMMAND_BUFFER_STATE_IN_RENDER_PASS,
    COMMAND_BUFFER_STATE_RECORDING_ENDED,
    COMMAND_BUFFER_STATE_SUBMITTED,
    COMMAND_BUFFER_STATE_NOT_ALLOCATED
} vulkan_command_buffer_state;

typedef struct vulkan_command_buffer {
    VkCommandBuffer handle;
    vulkan_command_buffer_state state;
} vulkan_command_buffer;

typedef struct vulkan_fence {
    VkFence handle;
    Boolean is_signaled;
} vulkan_fence;

typedef struct vulkan_shader_stage {
    VkShaderModuleCreateInfo create_info;
    VkShaderModule handle;
    VkPipelineShaderStageCreateInfo shader_state_create_info;
} vulkan_shader_stage;

typedef struct vulkan_pipeline {
    VkPipeline handle;
    VkPipelineLayout pipeline_layout;
} vulkan_pipeline;

#define OBJECT_SHADER_STAGE_COUNT 2
typedef struct vulkan_object_shader {
    vulkan_shader_stage stages[OBJECT_SHADER_STAGE_COUNT];
    
    VkDescriptorPool global_descriptor_pool;
    VkDescriptorSetLayout global_descriptor_set_layout;

    VkDescriptorSet global_descriptor_sets[3];

    global_uniform_object global_ubo;

    vulkan_buffer global_uniform_buffer;

    vulkan_pipeline pipeline;
} vulkan_object_shader;

typedef struct vulkan_context {
    UInt32 framebuffer_width;
    UInt32 framebuffer_height;
    UInt64 framebuffer_size_generation;
    UInt64 framebuffer_size_last_generation;

    VkInstance instance;
    VkAllocationCallbacks* allocator;
    VkSurfaceKHR surface;
    
#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif

    vulkan_device device;

    vulkan_swapchain swapchain;
    vulkan_renderpass main_renderpass;

    vulkan_buffer object_vertex_buffer;
    vulkan_buffer object_index_buffer;

    vulkan_command_buffer* graphics_command_buffers;

    VkSemaphore* image_available_semaphores;
    VkSemaphore* queue_complete_semaphores;

    UInt32 in_flight_fence_count;
    vulkan_fence* in_flight_fences;
    vulkan_fence** images_in_flight;

    UInt32 image_index;
    UInt32 current_frame;

    Boolean recreating_swapchain;

    vulkan_object_shader object_shader;

    UInt64 geometry_vertex_offset;
    UInt64 geometry_index_offset;

    Int32 (*find_memory_index)(UInt32 type_filter, UInt32 property_flags);
} vulkan_context;
