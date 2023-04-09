#pragma once

#include <cassert>
#include "vulkan/vulkan.h"

PFN_vkCreateDebugUtilsMessengerEXT VkCreateDebugUtilsMessengerExt = nullptr;
PFN_vkDestroyDebugUtilsMessengerEXT VkDestroyDebugUtilsMessengerExt = nullptr;

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance) {
    VkCreateDebugUtilsMessengerExt = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    VkDestroyDebugUtilsMessengerExt = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    
    if (!VkDestroyDebugUtilsMessengerExt || !VkCreateDebugUtilsMessengerExt) {
        assert(0);
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void LoadExtensionFunctions(VkInstance instance)
{
    CreateDebugUtilsMessengerEXT(instance);
}


