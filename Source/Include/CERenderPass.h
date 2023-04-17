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
        void Destroy(VkDevice device);

        VkDevice vDevice;
        VkRenderPass vRenderPass;
    };
}
