
//std
#include <cstdint>
#include <vector>

#include "vulkan/vulkan.h"

//CrystalEye
#include "Include/CEDevice.h"

//forward declarations for those in namespaces
struct SDL_Window;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool bEnableValidationLayers = false;
#else
    const bool bEnableValidationLayers = true;
#endif

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
