project("Selector" LANGUAGES CXX)
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include("Selector")
include("gtest")

find_package(Boost COMPONENTS chrono system program_options filesystem)

# Export compile comands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(source)
add_subdirectory(performance_tests)
add_subdirectory(tools)

enable_testing()
add_subdirectory(tests)

