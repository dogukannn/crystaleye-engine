#include <iostream>
#include "vulkan/vulkan.h"
#include "CEEngine.h"

int main(int argc, char* argv[])
{
    std::cout << "Welcome to the example application of the Crystal Eye Engine" << std::endl;
    CrystalEye::CEEngine Renderer;
    CrystalEye::StartConfig config;
    config.WindowHeight = 720;
    config.WindowWidth = 1280;
    
    Renderer.Initialize(config);
    
}
