#pragma once

#include <optional>
#include "CEVulkanCommon.h"

namespace CrystalEye 
{
   struct QueueFamilyIndices
   {
      std::optional<uint32_t> GraphicsFamily;

      bool IsComplete()
      {
         return GraphicsFamily.has_value();
      }
   };
   
   class CEDevice
   {
   public:
      CEDevice();
      void Initialize(VkInstance Instance);
      
      bool bInitialized = false;
      VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
      VkDevice Device = VK_NULL_HANDLE;
      VkInstance Instance = VK_NULL_HANDLE;
      VkQueue GraphicsQueue;
      
   private:
      VkPhysicalDevice GetSuitablePhysicalDevice(QueueFamilyIndices& outFamilyIndices);
      bool IsDeviceSuitable(VkPhysicalDevice physicalDevice, QueueFamilyIndices& outFamilyIndices);
      QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice physicalDevice);
      
   };
}
