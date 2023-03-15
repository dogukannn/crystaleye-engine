
add_executable(CrystalEyeEngine
	ExampleApplication.cpp
	CEEngine.cpp
	Include/CEEngine.h
	CEDevice.cpp
	Include/CEDevice.h
	VulkanExtensionFunctions.h)

set_property(TARGET CrystalEyeEngine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:CrystalEyeEngine>")

target_include_directories(CrystalEyeEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(CrystalEyeEngine vma glm tinyobjloader imgui stb_image)

target_link_libraries(CrystalEyeEngine Vulkan::Vulkan sdl2)
