
#include "Include/CEEngine.h"
#include <SDL.h>
#include <SDL_vulkan.h>
#include <iostream>
#include "Include/VulkanExtensionFunctions.h"
#include <complex>

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
#include "Include/CECommand.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

	if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

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
			startConfig.ApplicationName,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WindowExtent.width,
			WindowExtent.height,
			window_flags
		);
    	
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = startConfig.ApplicationName;
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
		std::vector<const char*> Extensions(SDLExtensions, SDLExtensions + SDLExtensionCount);
    	
    	if(bEnableValidationLayers)
    	{
    		//TODO check if layers available
    		createInfo.enabledLayerCount = validationLayers.size();
    		createInfo.ppEnabledLayerNames = validationLayers.data();

    		Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    	}
        else
        {
	        createInfo.enabledLayerCount = 0;
        }
		
    	Extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        createInfo.enabledExtensionCount = Extensions.size();
        createInfo.ppEnabledExtensionNames = Extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT MessengerCreateInfo{};
    	if(bEnableValidationLayers)
    	{
			MessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			MessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			MessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			MessengerCreateInfo.pfnUserCallback = debugCallback;
			MessengerCreateInfo.pUserData = nullptr; // Optional
    		createInfo.pNext = &MessengerCreateInfo;
    	}
		VK_CHECK(vkCreateInstance(&createInfo, nullptr, &Instance));
    	LoadExtensionFunctions(Instance);

    	if(bEnableValidationLayers)
    	{
			//create debug callback messenger
			VK_CHECK(VkCreateDebugUtilsMessengerExt(Instance, &MessengerCreateInfo, nullptr, &DebugMessenger));
    	}
		SDL_Vulkan_CreateSurface(Window, Instance, &Surface);

    	//init device
    	cDevice.Initialize(Instance, Surface, Window);

    	//init vma allocator
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice = cDevice.vPhysicalDevice;
		allocatorInfo.device = cDevice.vDevice;
		allocatorInfo.instance = Instance;
		vmaCreateAllocator(&allocatorInfo, &AllocatorVMA);

    	CreateSemaphore(cDevice.vDevice, vImageAvailableSemaphore);
    	CreateSemaphore(cDevice.vDevice, vRenderFinishedSemaphore);
    	CreateFence(cDevice.vDevice, vInFlightFence);
    	
		cCommandPool.Initialize(cDevice.vDevice, cDevice.vQueueFamilyIndices);
    	cMainCommandBuffer = cCommandPool.AllocateCommandBuffer();

    	cMainRenderPass.Initialize(cDevice.vDevice, cDevice.cSwapchain.vSwapchainImageFormat);
    	for(auto imageView : cDevice.cSwapchain.cSwapchainImageViews)
    	{
    		CEFrameBuffer frameBuffer;
    		std::vector<VkImageView> attachments = {imageView.vImageView};
    		frameBuffer.Initialize(cDevice.vDevice, cDevice.cSwapchain.vSwapchainExtent, cMainRenderPass.vRenderPass, attachments);
    		cSwapchainFrameBuffers.push_back(frameBuffer);
    	}

    	cMainPipeline.Initialize(cDevice.vDevice, cMainRenderPass.vRenderPass, cDevice.cSwapchain.vSwapchainExtent, cDevice.cSwapchain.vSwapchainImageFormat);
    }

    void CEEngine::Render()
    {
		//prepare meshes etc.
    	
    	RenderLoop();
    }

    void CEEngine::RenderLoop()
    {
		SDL_Event e;
		bool bQuit = false;
		//main loop
		while (!bQuit)
		{
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//close the window when user alt-f4s or clicks the X button			
				if (e.type == SDL_QUIT) bQuit = true;
			}

			DrawFrame();
		}
    }

    void CEEngine::DrawFrame()
    {
    	vkWaitForFences(cDevice.vDevice, 1, &vInFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(cDevice.vDevice, 1, &vInFlightFence);

		uint32_t imageIndex;
		vkAcquireNextImageKHR(cDevice.vDevice, cDevice.cSwapchain.vSwapchain, UINT64_MAX, vImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    	cMainCommandBuffer->Reset();
		cMainCommandBuffer->Begin();

    	//begin render pass
    	cMainRenderPass.Begin(cMainCommandBuffer->vCommandBuffer, cSwapchainFrameBuffers[imageIndex].vFrameBuffer, cDevice.cSwapchain.vSwapchainExtent);
    	
		vkCmdBindPipeline(cMainCommandBuffer->vCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, cMainPipeline.vPipeline);
    	VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(cDevice.cSwapchain.vSwapchainExtent.width);
        viewport.height = static_cast<float>(cDevice.cSwapchain.vSwapchainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(cMainCommandBuffer->vCommandBuffer, 0, 1, &viewport);
        
        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = cDevice.cSwapchain.vSwapchainExtent;
        vkCmdSetScissor(cMainCommandBuffer->vCommandBuffer, 0, 1, &scissor);

    	vkCmdDraw(cMainCommandBuffer->vCommandBuffer, 3, 1, 0, 0);
		cMainRenderPass.End(cMainCommandBuffer->vCommandBuffer);
    	cMainCommandBuffer->End();

    	std::vector<VkSemaphore> waitSemaphores = {vImageAvailableSemaphore};
    	std::vector<VkSemaphore> signalSemaphores = {vRenderFinishedSemaphore};
    	cMainCommandBuffer->Submit(cDevice.vGraphicsQueue, signalSemaphores, waitSemaphores, vInFlightFence);

    	VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        
        presentInfo.waitSemaphoreCount = signalSemaphores.size();
        presentInfo.pWaitSemaphores = signalSemaphores.data();
    	VkSwapchainKHR swapChains[] = {cDevice.cSwapchain.vSwapchain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
    	presentInfo.pResults = nullptr; // Optional

    	vkQueuePresentKHR(cDevice.vPresentQueue, &presentInfo);
    }

    void CEEngine::Destroy()
    {
    	vkWaitForFences(cDevice.vDevice, 1, &vInFlightFence, VK_TRUE, UINT64_MAX);
    	
		vkDestroySemaphore(cDevice.vDevice, vImageAvailableSemaphore, nullptr);
		vkDestroySemaphore(cDevice.vDevice, vRenderFinishedSemaphore, nullptr);
    	vkDestroyFence(cDevice.vDevice, vInFlightFence, nullptr);

    	cMainPipeline.Destroy(cDevice.vDevice);

    	for(auto frameBuffer : cSwapchainFrameBuffers)
    	{
    		frameBuffer.Destroy(cDevice.vDevice);
    	}
    	cMainRenderPass.Destroy(cDevice.vDevice);
    	
    	cCommandPool.Destroy(cDevice.vDevice);
			
    	vmaDestroyAllocator(AllocatorVMA);
    	
    	cDevice.Destroy();
    	
    	if (bEnableValidationLayers)
    	{
    		VkDestroyDebugUtilsMessengerExt(Instance, DebugMessenger, nullptr);
    	}
		 
        vkDestroySurfaceKHR(Instance, Surface, nullptr);

    	vkDestroyInstance(Instance, nullptr);
    	
		SDL_DestroyWindow(Window);
    }
}
