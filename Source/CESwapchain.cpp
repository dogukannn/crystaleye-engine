#include "Include/CESwapchain.h"
#include <algorithm>
#include <SDL.h>

namespace CrystalEye
{
    CESwapchain::CESwapchain()
    {
    }

    void CESwapchain::Initialize(VkDevice device, SwapChainSupportDetails details,
        QueueFamilyIndices queueFamilyIndices, SDL_Window* window, VkSurfaceKHR surface)
    {

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(details.presentModes);
        VkExtent2D extent = ChooseSwapExtent(window, details.capabilities);
        
        uint32_t imageCount = std::min(details.capabilities.maxImageCount, details.capabilities.minImageCount + 1);

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueIndices[] = {queueFamilyIndices.GraphicsFamily.value(), queueFamilyIndices.PresentFamily.value()};
        
        if (queueFamilyIndices.GraphicsFamily != queueFamilyIndices.PresentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }
        
        createInfo.preTransform = details.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;
        VK_CHECK(vkCreateSwapchainKHR(device, &createInfo, nullptr, &Swapchain));
    }

    void CESwapchain::Destroy(VkDevice device)
    {
        vkDestroySwapchainKHR(device, Swapchain, nullptr);
    }

    VkSurfaceFormatKHR CESwapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR CESwapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D CESwapchain::ChooseSwapExtent(SDL_Window* window, const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            SDL_GL_GetDrawableSize(window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                                            capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                                             capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }
}
