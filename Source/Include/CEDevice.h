#pragma once

#include <optional>
#include "CEVulkanCommon.h"
#include "CESwapchain.h"

class SDL_Window;
namespace CrystalEye 
{
   class CEDevice
   {
   public:
      CEDevice();
      void Initialize(VkInstance instance, VkSurfaceKHR surface, SDL_Window* window);
      void Destroy();
      
      CESwapchain cSwapchain;

      bool bInitialized = false;
      VkPhysicalDevice vPhysicalDevice = VK_NULL_HANDLE;
      VkSurfaceKHR vSurface;
      VkDevice vDevice = VK_NULL_HANDLE;
      VkInstance vInstance = VK_NULL_HANDLE;
      VkQueue vGraphicsQueue;
      VkQueue vPresentQueue;
      QueueFamilyIndices vQueueFamilyIndices;  
   private:
      VkPhysicalDevice GetSuitablePhysicalDevice(QueueFamilyIndices& outFamilyIndices);
      bool IsDeviceSuitable(VkPhysicalDevice physicalDevice, QueueFamilyIndices& outFamilyIndices);
      QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice physicalDevice);
      SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice);      
   };
}
