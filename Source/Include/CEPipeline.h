#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

#include "CERenderPass.h"

namespace CrystalEye
{
    class CEPipeline
    {
    public:
        CEPipeline();
        void Initialize(VkDevice device, VkExtent2D extent, VkFormat colorFormat);
        VkShaderModule CreateShaderModule(const std::vector<char>& code);
        void Destroy(VkDevice device);

        VkDevice vDevice;
        CERenderPass cRenderPass;
        VkPipelineLayout vPipelineLayout;
        VkPipeline vPipeline;
        VkShaderModule VertShaderModule; 
        VkShaderModule FragShaderModule; 
    };
}
