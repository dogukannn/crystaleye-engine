#pragma once

#include <vulkan/vulkan_core.h>

#include "CEVulkanCommon.h"

namespace CrystalEye
{
    class CECommandBuffer
    {
    public:
        CECommandBuffer();
        void Initialize(VkDevice device, VkCommandPool commandPool);
        void Begin();
        void End();
        void Reset();
        void Submit(VkQueue queue, std::vector<VkSemaphore>& signalSemaphores, std::vector<VkSemaphore>& waitSemaphores, VkFence waitFence);
        // void Destroy(VkDevice device);
        VkCommandBuffer vCommandBuffer;
    };
    
    class CECommandPool
    {
    public:
        CECommandPool();
        void Initialize(VkDevice device, QueueFamilyIndices queueFamilyIndices);
        void Destroy(VkDevice device);
        CECommandBuffer* AllocateCommandBuffer();
        VkDevice vDevice;
        VkCommandPool vCommandPool;
    };
    
}
