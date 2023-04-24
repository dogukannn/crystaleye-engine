#include <Include/CECommand.h>
#include <Include/CEVulkanCommon.h>

namespace CrystalEye
{
    //command buffer
    CECommandBuffer::CECommandBuffer()
    {
    }

    void CECommandBuffer::Initialize(VkDevice device, VkCommandPool commandPool)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        VK_CHECK(vkAllocateCommandBuffers(device, &allocInfo, &vCommandBuffer));
    }

    void CECommandBuffer::Begin()
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional
        VK_CHECK(vkBeginCommandBuffer(vCommandBuffer, &beginInfo));
    }

    void CECommandBuffer::End()
    {
        VK_CHECK(vkEndCommandBuffer(vCommandBuffer));
    }

    void CECommandBuffer::Reset()
    {
        VK_CHECK(vkResetCommandBuffer(vCommandBuffer, 0));
    }

    void CECommandBuffer::Submit(VkQueue queue, std::vector<VkSemaphore>& signalSemaphores, std::vector<VkSemaphore>& waitSemaphores, VkFence waitFence)
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = waitSemaphores.size();
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vCommandBuffer;
        submitInfo.signalSemaphoreCount = signalSemaphores.size();
        submitInfo.pSignalSemaphores = signalSemaphores.data();
        VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, waitFence));
    }


    //command pool
    CECommandPool::CECommandPool()
    {
    }

    void CECommandPool::Initialize(VkDevice device, QueueFamilyIndices queueFamilyIndices)
    {
        vDevice = device;
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

        VK_CHECK(vkCreateCommandPool(device, &poolInfo, nullptr, &vCommandPool));
    }

    void CECommandPool::Destroy(VkDevice device)
    {
        vkDestroyCommandPool(device, vCommandPool, nullptr);
    }

    CECommandBuffer* CECommandPool::AllocateCommandBuffer()
    {
        auto commandBuffer = new CECommandBuffer();
        commandBuffer->Initialize(vDevice, vCommandPool);
        return commandBuffer;
    }
}
