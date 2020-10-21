execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git log -1 --format=%H
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git log -1 --format="%aN <%aE>"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_AUTHOR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git log -1 --format="%aN <%aE>"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_AUTHOR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git log -1 --format="%s"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_SUBJECT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Default Pagoda version, build number and date
if (NOT DEFINED PAGODA_VERSION)
    set(PAGODA_VERSION "${CMAKE_PROJECT_VERSION}")
endif()

if (NOT DEFINED PAGODA_BUILD_NUMBER)
    set(PAGODA_BUILD_NUMBER "0")
endif()

string(TIMESTAMP PAGODA_BUILD_DATE "%d/%m/%Y %H:%M:%S")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    option(PAGODA_PROFILER_ACTIVE "Enable or disable built in profiling" OFF)
    option(PAGODA_ENABLE_ASSERTIONS "Enable or disable assertions" OFF)
else()
    option(PAGODA_PROFILER_ACTIVE "Enable or disable built in profiling" ON)
    option(PAGODA_ENABLE_ASSERTIONS "Enable or disable assertions" ON)
endif()

string(REPLACE "\"" "" GIT_COMMIT_SUBJECT "${GIT_COMMIT_SUBJECT}")
string(REPLACE "#" "" GIT_COMMIT_SUBJECT "${GIT_COMMIT_SUBJECT}")
string(REPLACE "\"" "" GIT_COMMIT_AUTHOR "${GIT_COMMIT_AUTHOR}")
string(REPLACE "#" "" GIT_COMMIT_AUTHOR "${GIT_COMMIT_AUTHOR}")
string(REPLACE "\"" "" GIT_BRANCH "${GIT_BRANCH}")
string(REPLACE "#" "" GIT_BRANCH "${GIT_BRANCH}")
set(GIT_COMMIT_SUBJECT "\"${GIT_COMMIT_SUBJECT}\"")
set(GIT_COMMIT_AUTHOR "\"${GIT_COMMIT_AUTHOR}\"")

set(PAGODA_INCLUDE_GIT_INFO 0)
if ("${GIT_BRANCH}" STREQUAL "master")
    option(PAGODA_GIT_INFO "Include git information in the version information" OFF)
elseif("${GIT_BRANCH}" MATCHES "^release-.+")
    option(PAGODA_GIT_INFO "Include git information in the version information" OFF)
else()
    option(PAGODA_GIT_INFO "Include git information in the version information" ON)
endif()

message(STATUS "Version:            ${PAGODA_VERSION}")
message(STATUS "Build number:       ${PAGODA_BUILD_NUMBER}")
message(STATUS "Build date:         ${PAGODA_BUILD_DATE}")
message(STATUS "Profiler Active:    ${PAGODA_PROFILER_ACTIVE}")
message(STATUS "Assertions Active:  ${PAGODA_ENABLE_ASSERTIONS}")
if (${PAGODA_GIT_INFO})
    message(STATUS "Git Branch:         ${GIT_BRANCH}")
    message(STATUS "Git Commit Hash:    ${GIT_COMMIT_HASH}")
    message(STATUS "Git Commit Author:  ${GIT_COMMIT_AUTHOR}")
    message(STATUS "Git Commit Subject: ${GIT_COMMIT_SUBJECT}")
    set(PAGODA_INCLUDE_GIT_INFO 1)
endif()


################################################
# Helpers
################################################
function (add_unit_test unit_test_src)
    get_filename_component(unit_test_base_name ${unit_test_src} NAME)
    string(REPLACE ".test.cpp" "_test" test_name ${unit_test_base_name})

    add_executable(${test_name} ${unit_test_src})

    target_include_directories(
        ${test_name}
        PRIVATE
            $<INSTALL_INTERFACE:pagoda>
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
            ${Boost_INCLUDE_DIRS}
    )

    target_compile_features(
        ${test_name}
        PRIVATE
            cxx_std_17
    )

    target_compile_options(
        ${test_name}
        PRIVATE
            -Wall
            $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
            $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
    )

    target_compile_definitions(
        ${test_name}
        PUBLIC
          ${PAGODA_COMPILER_ID}
        PRIVATE
            $<$<CONFIG:DEBUG>:DEBUG>
    )

    target_link_libraries(
        ${test_name}
        PRIVATE
            libpagoda
            Boost::chrono
            Boost::system
            Boost::filesystem
            gmock
            gmock_main
            gtest
            gtest_main
            pthread
    )

    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
  set(PAGODA_COMPILER_ID "-DPAGODA_COMPILER_CLANG")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  set(PAGODA_COMPILER_ID "-DPAGODA_COMPILER_GCC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  set(PAGODA_COMPILER_ID "-DPAGODA_COMPILER_MSVC")
endif()
