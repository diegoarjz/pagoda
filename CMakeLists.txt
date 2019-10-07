project("Selector" LANGUAGES CXX)
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include("Selector")
include("gtest")

find_package(Boost COMPONENTS chrono system program_options filesystem)

# Export compile comands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Grab selscript
include(ExternalProject)

set(SELSCRIPT_ROOT ${CMAKE_BINARY_DIR}/selscript)
set(SELSCRIPT_INSTALL_DIR ${SELSCRIPT_ROOT}/install)
set(SELSCRIPT_LIB_DIR ${SELSCRIPT_INSTALL_DIR}/lib/selscript)
set(SELSCRIPT_INCLUDE_DIR ${SELSCRIPT_INSTALL_DIR}/include/selscript)

ExternalProject_Add(selscript_external
    PREFIX ${SELSCRIPT_ROOT}
    GIT_REPOSITORY "https://github.com/diegoarjz/selscript"
    GIT_TAG "master"
    UPDATE_COMMAND ""
    BINARY_DIR ${SELSCRIPT_ROOT}/source
    SOURCE_DIR ${SELSCRIPT_ROOT}/source
    INSTALL_DIR ${SELSCRIPT_INSTALL_DIR}
    CONFIGURE_COMMAND cmake -DCMAKE_INSTALL_PREFIX=${SELSCRIPT_INSTALL_DIR}
    BUILD_COMMAND "make"
)
add_library(selscript STATIC IMPORTED)
set_target_properties(selscript PROPERTIES IMPORTED_LOCATION ${SELSCRIPT_LIB_DIR}/libselscript.a)
add_dependencies(selscript selscript_external)

add_subdirectory(source)
add_subdirectory(performance_tests)
add_subdirectory(tools)

enable_testing()
add_subdirectory(tests)

