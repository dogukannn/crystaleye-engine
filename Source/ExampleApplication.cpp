#include <iostream>
#include "vulkan/vulkan.h"
#include "Include/CEEngine.h"

int main(int argc, char* argv[])
{
    std::cout << "Welcome to the example application of the Crystal Eye Engine" << std::endl;
    CrystalEye::CEEngine Renderer;
    CrystalEye::StartConfig config = {};
    config.WindowHeight = 720;
    config.WindowWidth = 1280;
    config.ApplicationName = "Example Application";
    
    Renderer.Initialize(config);
    Renderer.Destroy(); 
}
