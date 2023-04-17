#pragma once

#include <vulkan/vulkan_core.h>

namespace CrystalEye
{
    class CEImageView
    {
    public:
        CEImageView();
        void Initialize(VkDevice device, VkImage image, VkFormat format);
        void Destroy(VkDevice device);
        VkImageView vImageView;
    };
}
