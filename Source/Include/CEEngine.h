#pragma once

//std
#include <cstdint>
#include <vector>

//CrystalEye
#include "Include/CEDevice.h"
#include "CEVulkanCommon.h"

//forward declarations for those in namespaces
struct SDL_Window;

namespace CrystalEye 
{
    struct StartConfig
    {
        uint32_t WindowHeight; 
        uint32_t WindowWidth; 
    };

    class CEEngine
    {
    public:
        CEEngine();
        void Initialize(StartConfig& startConfig);

        void Destroy();
        
    private:
        SDL_Window* Window{ nullptr };
        VkExtent2D WindowExtent{ 1280 , 720 };
        VkInstance Instance;
        VkDebugUtilsMessengerEXT DebugMessenger;
        CEDevice Device;
        
    };
    
}
