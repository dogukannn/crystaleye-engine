#pragma once

#include <vector>
#include <iostream>
#include <optional>
#include "vulkan/vulkan.h"

#define VK_CHECK(x) if((x) != VK_SUCCESS) \
	{									\
		std::cout << "Vulkan command failed with " << x << std::endl; \
		exit(0);						\
	}

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
    const bool bEnableValidationLayers = false;
#else
    const bool bEnableValidationLayers = true;
#endif

namespace CrystalEye
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> GraphicsFamily;
        std::optional<uint32_t> PresentFamily;

        bool IsComplete()
        {
            return GraphicsFamily.has_value() && PresentFamily.has_value();
        }
    };
    
    static void CreateSemaphore(VkDevice device, VkSemaphore& outSemaphore)
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &outSemaphore));
    }
    
    static void CreateFence(VkDevice device, VkFence& outFence)
    {
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        VK_CHECK(vkCreateFence(device, &fenceInfo, nullptr, &outFence));
    }
}

