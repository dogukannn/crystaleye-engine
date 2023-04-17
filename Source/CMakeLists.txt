
add_executable(CrystalEyeEngine
	ExampleApplication.cpp
	CEEngine.cpp
	Include/CEEngine.h
	CEDevice.cpp
	Include/CEDevice.h
	Include/VulkanExtensionFunctions.h
	Include/CEVulkanCommon.h
	Include/CESwapchain.h
	CESwapchain.cpp
	Include/CEImage.h
	CEImage.cpp
	Include/CEPipeline.h
	CEPipeline.cpp
	Include/CERenderPass.h
	CERenderPass.cpp
	Include/CEFrameBuffer.h
	CEFrameBuffer.cpp
	Include/CECommand.h
	CECommand.cpp)

set_property(TARGET CrystalEyeEngine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:CrystalEyeEngine>")

target_include_directories(CrystalEyeEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(CrystalEyeEngine vma glm tinyobjloader imgui stb_image)

target_link_libraries(CrystalEyeEngine Vulkan::Vulkan sdl2)
