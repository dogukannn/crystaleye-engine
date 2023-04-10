#CMake project for Crystal Eye Engine

cmake_minimum_required(VERSION 3.25)

project("CrystalEyeEngine")

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)

add_subdirectory(External)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Executables")

add_subdirectory(Source)
