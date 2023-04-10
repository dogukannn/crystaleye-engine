#include "Include/CEImage.h"

#include "Include/CEVulkanCommon.h"

namespace CrystalEye 
{
    CEImageView::CEImageView()
    {
        
    }

    void CEImageView::Initialize(VkDevice device, VkImage image, VkFormat format)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.image = image;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;
        createInfo.components = {   .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                                    .a = VK_COMPONENT_SWIZZLE_IDENTITY};
        
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VK_CHECK(vkCreateImageView(device, &createInfo, nullptr, &vImageView));
    }

    void CEImageView::Destroy(VkDevice device)
    {
        vkDestroyImageView(device, vImageView, nullptr);
    }
}


