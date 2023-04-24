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
        void Initialize(VkDevice device, VkRenderPass renderPass, VkExtent2D extent, VkFormat colorFormat);
        VkShaderModule CreateShaderModule(const std::vector<uint32_t>& code);
        void Destroy(VkDevice device);

        VkDevice vDevice;
        VkPipelineLayout vPipelineLayout;
        VkPipeline vPipeline;
        VkShaderModule VertShaderModule; 
        VkShaderModule FragShaderModule; 
    };
}
