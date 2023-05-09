#pragma once

//std
#include <cstdint>
#include <vector>

//CrystalEye
#include <vk_mem_alloc.h>

#include "CECommand.h"
#include "CEPipeline.h"
#include "CERenderPass.h"
#include "Include/CESwapchain.h"
#include "Include/CEDevice.h"
#include "CEVulkanCommon.h"

const int MAX_FRAMES_IN_FLIGHT = 2;
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
        void PreparePresentObjects();
        void Render();
        void RenderLoop();
        void DrawFrame(); 
        void Destroy();
        
    private:
        //Crystal Eye Classes
        CEDevice cDevice;
        CECommandPool cCommandPool;
        std::vector<CECommandBuffer*> cMainCommandBuffers;
        CERenderPass cMainRenderPass;
        std::vector<CEFrameBuffer> cSwapchainFrameBuffers;
        CEPipeline cMainPipeline;
        //SDL Classes
        SDL_Window* Window{ nullptr };

        //Vulkan Classes
        VkExtent2D WindowExtent{ 1280 , 720 };
        VkInstance Instance;
        VkDebugUtilsMessengerEXT DebugMessenger;
        VkSurfaceKHR Surface;
        std::vector<VkSemaphore> vImageAvailableSemaphores;
        std::vector<VkSemaphore> vRenderFinishedSemaphores;
        std::vector<VkFence> vInFlightFences;
        VkViewport vCurrentViewport;
        VkRect2D vCurrentScissor;
        VkPresentInfoKHR vPresentInfo;
        //VMA Classes
        VmaAllocator AllocatorVMA;
    };
    
}
