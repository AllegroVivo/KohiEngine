#pragma once

#include "defines.h"
#include <core/asserts.h>

#include <vulkan/vulkan.h>

#define VK_CHECK(expr)               \
    {                                \
        KASSERT(expr == VK_SUCCESS); \
    }

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

typedef struct vulkan_swapchain {
    VkSurfaceFormatKHR image_format;
    UInt8 max_frames_in_flight;
    VkSwapchainKHR handle;
    UInt32 image_count;
    VkImage* images;
    VkImageView* views;

    vulkan_image depth_attachment;
} vulkan_swapchain; 

typedef struct vulkan_context {
    UInt32 framebuffer_width;
    UInt32 framebuffer_height;

    VkInstance instance;
    VkAllocationCallbacks* allocator;
    VkSurfaceKHR surface;
    
#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif

    vulkan_device device;

    vulkan_swapchain swapchain;
    UInt32 image_index;
    UInt32 current_frame;

    Boolean recreating_swapchain;

    Int32 (*find_memory_index)(UInt32 type_filter, UInt32 property_flags);

} vulkan_context;
