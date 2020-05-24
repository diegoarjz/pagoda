cmake_policy(SET CMP0048 NEW)

project("Pagoda" LANGUAGES CXX VERSION 0.1.0)
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include("Pagoda")
include("gtest")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS chrono system program_options filesystem)

find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(OpenGL REQUIRED)
find_library(COCOA_LIBRARY Cocoa)
find_library(IOKIT_LIBRARY IOKit)
find_library(CORE_VIDEO_LIBRARY CoreVideo)

# Export compile comands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
