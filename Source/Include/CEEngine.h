#pragma once

//std
#include <cstdint>
#include <vector>

//CrystalEye
#include <vk_mem_alloc.h>

#include "Include/CESwapchain.h"
#include "Include/CEDevice.h"
#include "CEVulkanCommon.h"

//forward declarations for those in namespaces
struct SDL_Window;

namespace CrystalEye 
{
    struct StartConfig
    {
        uint32_t WindowWidth; 
        uint32_t WindowHeight;
        const char* ApplicationName;
    };

    class CEEngine
    {
    public:
        CEEngine();
        void Initialize(StartConfig& startConfig);

        void Destroy();
        
    private:
        //Crystal Eye Classes
        CrystalEye::CEDevice cDevice;

        //SDL Classes
        SDL_Window* Window{ nullptr };

        //Vulkan Classes
        VkExtent2D WindowExtent{ 1280 , 720 };
        VkInstance Instance;
        VkDebugUtilsMessengerEXT DebugMessenger;
        VkSurfaceKHR Surface;

        //VMA Classes
        VmaAllocator AllocatorVMA;
    };
    
}
