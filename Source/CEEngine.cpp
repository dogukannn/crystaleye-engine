
#include "CEEngine.h"
#include <SDL.h>
#include <SDL_vulkan.h>
#include <iostream>

#define VK_CHECK(x) if(x != VK_SUCCESS) \
	{									\
		std::cout << "Vulkan command failed with " << x << std::endl; \
		exit(0);						\
	}									\

		



namespace CrystalEye 
{
    CEEngine::CEEngine()
    {
        
    }

    void CEEngine::Initialize(StartConfig& startConfig)
    {
    	WindowExtent.height = startConfig.WindowHeight;
    	WindowExtent.width = startConfig.WindowWidth;
    	
		SDL_Init(SDL_INIT_VIDEO);
	
		SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);
		
		Window = SDL_CreateWindow(
			"Vulkan Engine",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WindowExtent.width,
			WindowExtent.height,
			window_flags
		);
    	
		VkInstance instance;
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Example Application";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "CrystalEyeEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

    	uint32_t SDLExtensionCount = 0;
    	SDL_Vulkan_GetInstanceExtensions(Window, &SDLExtensionCount, nullptr);
        const char** SDLExtensions = new const char*[SDLExtensionCount];
    	SDL_Vulkan_GetInstanceExtensions(Window, &SDLExtensionCount, SDLExtensions);
        
        createInfo.enabledExtensionCount = SDLExtensionCount;
        createInfo.ppEnabledExtensionNames = SDLExtensions;
    	
		VK_CHECK(vkCreateInstance(&createInfo, nullptr, &instance));
    }
}
