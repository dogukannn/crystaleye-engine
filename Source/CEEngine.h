
#include <cstdint>
#include "vulkan/vulkan.h"

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

    private:
        SDL_Window* Window{ nullptr };
        VkExtent2D WindowExtent{ 1280 , 720 };
    };
    
}
