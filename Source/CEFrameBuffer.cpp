#include "Include/CEFrameBuffer.h"

#include <vector>

#include "Include/CEVulkanCommon.h"

namespace CrystalEye
{
    CEFrameBuffer::CEFrameBuffer()
    {
    }

    void CEFrameBuffer::Initialize(VkDevice device, VkExtent2D extent, VkRenderPass renderPass, std::vector<VkImageView>& attachments)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        VK_CHECK(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &vFrameBuffer));
    }

    void CEFrameBuffer::Destroy(VkDevice device)
    {
        vkDestroyFramebuffer(device, vFrameBuffer, nullptr);
    }
}
