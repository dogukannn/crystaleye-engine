#include "Include/CEDevice.h"
#include <iostream>
#include <vector>
#include <set>

namespace CrystalEye 
{
    CEDevice::CEDevice()
    {
    }

    void CEDevice::Initialize(VkInstance instance, VkSurfaceKHR surface, SDL_Window* window)
    {
        vInstance = instance;
        vSurface = surface;
        QueueFamilyIndices familyIndices = {};
        vPhysicalDevice = GetSuitablePhysicalDevice(familyIndices);
        vQueueFamilyIndices = familyIndices;
        
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> queueIndices;
        queueIndices.insert(familyIndices.GraphicsFamily.value());
        queueIndices.insert(familyIndices.PresentFamily.value());

        float queuePriority = 1.0f;
        for(auto index : queueIndices)
        {
            VkDeviceQueueCreateInfo queueCreateInfo;
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.flags = 0;
            queueCreateInfo.pNext = nullptr;
            queueCreateInfo.queueFamilyIndex = index;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        
        if (bEnableValidationLayers) {
            deviceCreateInfo.enabledLayerCount = validationLayers.size();
            deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            deviceCreateInfo.enabledLayerCount = 0;
        }

        VK_CHECK(vkCreateDevice(vPhysicalDevice, &deviceCreateInfo, nullptr, &vDevice));

        vkGetDeviceQueue(vDevice, familyIndices.GraphicsFamily.value(), 0, &vGraphicsQueue);
        vkGetDeviceQueue(vDevice, familyIndices.PresentFamily.value(), 0, &vPresentQueue);

        cSwapchain.Initialize(vDevice, QuerySwapChainSupport(vPhysicalDevice), vQueueFamilyIndices, window, surface);
        
        bInitialized = true;
    }

    void CEDevice::Destroy()
    {
        cSwapchain.Destroy(vDevice);
        
        vkDestroyDevice(vDevice, nullptr);
    }

    VkPhysicalDevice CEDevice::GetSuitablePhysicalDevice(QueueFamilyIndices& outFamilyIndices)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vInstance, &deviceCount, nullptr);
        if(deviceCount == 0)
        {
            throw std::runtime_error("No suitable device is found.");
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(vInstance, &deviceCount, devices.data());

        for(auto device : devices)
        {
            if(IsDeviceSuitable(device, outFamilyIndices))
            {
                return device;
            }
        }
        throw std::runtime_error("No suitable device is found.");
    }
    
    bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }
    
    bool CEDevice::IsDeviceSuitable(VkPhysicalDevice physicalDevice, QueueFamilyIndices& outFamilyIndices)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

        bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice); 
        
        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }
        
        outFamilyIndices = GetQueueFamilies(physicalDevice);
        //TODO implement a way to get non-discrete GPU but prioritizing discrete one
        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
            && deviceFeatures.geometryShader
            && extensionsSupported
            && swapChainAdequate
            && outFamilyIndices.IsComplete();
    }

    QueueFamilyIndices CEDevice::GetQueueFamilies(VkPhysicalDevice physicalDevice)
    {
        QueueFamilyIndices familyIndices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        int index = 0;
        for(const auto& queueFamily : queueFamilies)
        {
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                familyIndices.GraphicsFamily = index;
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, vSurface, &presentSupport);
            if(presentSupport)
            {
                familyIndices.PresentFamily = index;
            }
            if(familyIndices.IsComplete())
            {
                break;
            }
            index++;
        }
        return familyIndices;
    }

    SwapChainSupportDetails CEDevice::QuerySwapChainSupport(VkPhysicalDevice physicalDevice)
    {
        SwapChainSupportDetails result;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, vSurface, &result.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, vSurface, &formatCount, nullptr);
        
        if (formatCount != 0) {
            result.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, vSurface, &formatCount, result.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, vSurface, &presentModeCount, nullptr);
        
        if (presentModeCount != 0) {
            result.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, vSurface, &presentModeCount, result.presentModes.data());
        }
        
        return result;
    }
}
