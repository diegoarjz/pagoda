cmake_policy(SET CMP0048 NEW)

set(version 0.1.0)

project("Pagoda" LANGUAGES CXX VERSION ${version})
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

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
find_package(GLEW REQUIRED)

enable_testing()
add_subdirectory(include)
add_subdirectory(src)

#add_subdirectory(tests)

include(GNUInstallDirs)
install(EXPORT pagoda-export
        DESTINATION ${CMAKE_INSTALL_LIBDIR})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/pagoda-config.cmake.in
  ${CMAKE_BINARY_DIR}/pagoda-config.cmake @ONLY)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/pagoda-config-version.cmake.in
  ${CMAKE_BINARY_DIR}/pagoda-config-version.cmake @ONLY)

install(
  FILES
    ${CMAKE_BINARY_DIR}/pagoda-config.cmake
    ${CMAKE_BINARY_DIR}/pagoda-config-version.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR})
