#include "Include/CEDevice.h"
#include <iostream>
#include <vector>

namespace CrystalEye 
{
    CEDevice::CEDevice()
    {
    }

    void CEDevice::Initialize(VkInstance instance)
    {
        Instance = instance;
        QueueFamilyIndices familyIndices;
        PhysicalDevice = GetSuitablePhysicalDevice(familyIndices);

        VkDeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = familyIndices.GraphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount = 0;
        
        if (bEnableValidationLayers) {
            deviceCreateInfo.enabledLayerCount = validationLayers.size();
            deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            deviceCreateInfo.enabledLayerCount = 0;
        }

        VK_CHECK(vkCreateDevice(PhysicalDevice, &deviceCreateInfo, nullptr, &Device));

        vkGetDeviceQueue(Device, familyIndices.GraphicsFamily.value(), 0, &GraphicsQueue);
        
        bInitialized = true;
    }

    VkPhysicalDevice CEDevice::GetSuitablePhysicalDevice(QueueFamilyIndices& outFamilyIndices)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr);
        if(deviceCount == 0)
        {
            throw std::runtime_error("No suitable device is found.");
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(Instance, &deviceCount, devices.data());

        for(auto device : devices)
        {
            if(IsDeviceSuitable(device, outFamilyIndices))
            {
                return device;
            }
        }
        throw std::runtime_error("No suitable device is found.");
    }

    bool CEDevice::IsDeviceSuitable(VkPhysicalDevice physicalDevice, QueueFamilyIndices& outFamilyIndices)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

        outFamilyIndices = GetQueueFamilies(PhysicalDevice);
        //TODO implement a way to get non-discrete GPU but prioritizing discrete one
        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
            && deviceFeatures.geometryShader
            && outFamilyIndices.IsComplete();
    }

    QueueFamilyIndices CEDevice::GetQueueFamilies(VkPhysicalDevice physicalDevice)
    {
        QueueFamilyIndices familyIndices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, queueFamilies.data());

        int index = 0;
        for(const auto& queueFamily : queueFamilies)
        {
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                familyIndices.GraphicsFamily = index;
            }
            if(familyIndices.IsComplete())
            {
                break;
            }
            index++;
        }
        return familyIndices;
    }
}
