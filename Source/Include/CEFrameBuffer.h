#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace CrystalEye
{
    class CEFrameBuffer
    {
    public:
        CEFrameBuffer();
        void Initialize(VkDevice device, VkExtent2D extent, VkRenderPass renderPass, std::vector<VkImageView>& attachments);
        void Destroy(VkDevice device);

        VkFramebuffer vFrameBuffer;
    };
    
}
