cmake_policy(SET CMP0048 NEW)

project("Pagoda" LANGUAGES CXX VERSION 0.1.0)
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include("Pagoda")
include("gtest")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS chrono system program_options filesystem)

find_package(PNG REQUIRED)

message(STATUS ">>>> ${PNG_INCLUDE_DIRS}")

# Export compile comands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()
add_subdirectory(src)
add_subdirectory(tests)
