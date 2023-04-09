#pragma once

#include "CEVulkanCommon.h"

class SDL_Window;

namespace CrystalEye 
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    
    class CESwapchain 
    {
    public:
        CESwapchain();
        
        void Initialize(VkDevice device, SwapChainSupportDetails details,
            QueueFamilyIndices queueFamilyIndices,
            SDL_Window* window,
            VkSurfaceKHR surface);
        
        void Destroy(VkDevice device);
        
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(SDL_Window* window, const VkSurfaceCapabilitiesKHR& capabilities);
        VkSwapchainKHR Swapchain;
    private:
    };
    
}
