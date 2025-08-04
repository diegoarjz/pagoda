cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0091 NEW)

set(version 0.1.0)

cmake_minimum_required(VERSION 3.30.0 FATAL_ERROR)
project("Pagoda" LANGUAGES CXX VERSION ${version})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

include("Pagoda")
include("WGS84toCartesian")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS chrono system program_options filesystem)
find_package(GTest)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
find_package(EnTT REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(magic_enum REQUIRED)
find_package(fmt REQUIRED)

enable_testing()
add_subdirectory(include)
add_subdirectory(src)

add_subdirectory(tests)
