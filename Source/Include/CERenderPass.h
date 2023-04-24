#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace CrystalEye
{
    class CERenderPass
    {
    public:
        CERenderPass();
        void Initialize(VkDevice device, VkFormat colorFormat);
        void Begin(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, VkExtent2D extent);
        void End(VkCommandBuffer commandBuffer);
        void Destroy(VkDevice device);

        VkDevice vDevice;
        VkRenderPass vRenderPass;
    };
}
