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

# Adds a build unit (library, executable, etc).
# Only meant to be used by other functions here
# Example Usage
#   add_pagoda_build_unit(
#     TYPE                 <OBJECT,EXECUTABLE>
#     NAME                 <module name>
#     SOURCES              <list of source files>
#     DEPENDENCIES         <list of dependencies>
#     COMPILE_DEFINITIONS  <list of compiler definitions>
#  )
function (add_pagoda_build_unit)
  set(multiValue SOURCES DEPENDENCIES COMPILE_DEFINITIONS)
  set(singleValue NAME TYPE)
  include(CMakeParseArguments)
  cmake_parse_arguments(
      PARSED_ARGS       # prefix of output variables
      ""                # list of names of the boolean arguments (only defined ones will be true)
      "${singleValue}"  # list of names of mono-valued arguments
      "${multiValue}"   # list of names of multi-valued arguments (output variables are lists)
      ${ARGN}           # arguments of the function to parse, here we take the all original ones
  )
  # note: remaining unparsed arguments can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS
  if(NOT PARSED_ARGS_NAME)
    message(FATAL_ERROR "You must provide a name")
  endif(NOT PARSED_ARGS_NAME)

  if (${PARSED_ARGS_TYPE} STREQUAL "OBJECT")
    add_library(${PARSED_ARGS_NAME} OBJECT ${PARSED_ARGS_SOURCES})
  elseif (${PARSED_ARGS_TYPE} STREQUAL "EXECUTABLE")
    add_executable(${PARSED_ARGS_NAME} ${PARSED_ARGS_SOURCES})
  elseif (${PARSED_ARGS_TYPE} STREQUAL "LIBRARY")
    add_library(${PARSED_ARGS_NAME} ${PARSED_ARGS_SOURCES})
  else ()
    message(FATAL_ERROR "Unknown type ${PARSED_ARGS_TYPE}")
  endif()

  target_compile_features(
    ${PARSED_ARGS_NAME}
    PRIVATE
      cxx_std_17
  )

  target_compile_options(
    ${PARSED_ARGS_NAME}
    PRIVATE
      # We don't treat warnings as errors on windows because Boost.Spirit
      # produces a lot of warnings on windows.
      $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>
      $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
      $<$<CXX_COMPILER_ID:Clang>:-Wno-gnu-zero-variadic-macro-arguments>
  )

  target_include_directories(
    ${PARSED_ARGS_NAME}
    PUBLIC
      $<INSTALL_INTERFACE:pagoda>
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    PRIVATE
      ${Boost_INCLUDE_DIRS}
  )

  target_compile_definitions(
    ${PARSED_ARGS_NAME}
    PRIVATE
      $<$<CONFIG:DEBUG>:DEBUG>
      # Set compiler id
      $<$<CXX_COMPILER_ID:GNU>:PAGODA_COMPILER_GCC>
      $<$<CXX_COMPILER_ID:Clang,AppleClang>:PAGODA_COMPILER_CLANG>
      $<$<CXX_COMPILER_ID:MSVC>:PAGODA_COMPILER_MSVC>
      # Set OS id
      $<$<PLATFORM_ID:Darwin>:PAGODA_OS_MACOS>
      $<$<PLATFORM_ID:Linux>:PAGODA_OS_LINUX>
      $<$<PLATFORM_ID:Windows>:PAGODA_OS_WINDOWS>
      ${PARSED_ARGS_COMPILE_DEFINITIONS}
  )

  target_link_libraries(
    ${PARSED_ARGS_NAME}
    PRIVATE
      Boost::filesystem
      ${PARSED_ARGS_DEPENDENCIES}
  )

  if (${PARSED_ARGS_TYPE} STREQUAL "EXECUTABLE")
    include(GNUInstallDirs)
    install(TARGETS ${PARSED_ARGS_NAME}
      EXPORT pagoda-export
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
  endif()
endfunction()

# Adds a pagoda module
# Example Usage
#   add_pagoda_module(
#     NAME                 <module name>
#     SOURCES              <list of source files>
#     DEPENDENCIES         <list of dependencies>
#     COMPILE_DEFINITIONS  <list of compiler definitions>
#  )
function (add_pagoda_module)
  add_pagoda_build_unit(TYPE OBJECT ${ARGN})
endfunction()

# Adds an executable linked against the pagoda and other libs.
# Example Usage
#   add_pagoda_executable(
#     NAME                 <module name>
#     SOURCES              <list of source files>
#     DEPENDENCIES         <list of dependencies>
#     COMPILE_DEFINITIONS  <list of compiler definitions>
#  )
function (add_pagoda_executable)

  set(multiValue SOURCES DEPENDENCIES COMPILE_DEFINITIONS)
  set(singleValue NAME)
  include(CMakeParseArguments)
  cmake_parse_arguments(
      PARSED_ARGS       # prefix of output variables
      ""                # list of names of the boolean arguments (only defined ones will be true)
      "${singleValue}"  # list of names of mono-valued arguments
      "${multiValue}"   # list of names of multi-valued arguments (output variables are lists)
      ${ARGN}           # arguments of the function to parse, here we take the all original ones
  )
  # note: remaining unparsed arguments can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS

  if(NOT PARSED_ARGS_NAME)
    message(FATAL_ERROR "You must provide a name")
  endif(NOT PARSED_ARGS_NAME)

  add_pagoda_build_unit(
    TYPE                EXECUTABLE
    NAME                ${PARSED_ARGS_NAME}
    SOURCES             ${PARSED_ARGS_SOURCES}
    DEPENDENCIES        libpagoda ${PARSED_ARGS_DEPENDENCIES}
    COMPILE_DEFINITIONS ${PARSED_ARGS_COMPILE_DEFINITIONS}
  )

endfunction()

# Adds a library to pagoda
# Example Usage
#   add_pagoda_library(
#     NAME                 <module name>
#     SOURCES              <list of source files>
#     DEPENDENCIES         <list of dependencies>
#     COMPILE_DEFINITIONS  <list of compiler definitions>
#  )
function (add_pagoda_library)
  set(multiValue SOURCES DEPENDENCIES COMPILE_DEFINITIONS)
  set(singleValue NAME)
  include(CMakeParseArguments)
  cmake_parse_arguments(
      PARSED_ARGS       # prefix of output variables
      ""                # list of names of the boolean arguments (only defined ones will be true)
      "${singleValue}"  # list of names of mono-valued arguments
      "${multiValue}"   # list of names of multi-valued arguments (output variables are lists)
      ${ARGN}           # arguments of the function to parse, here we take the all original ones
  )
  # note: remaining unparsed arguments can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS

  if(NOT PARSED_ARGS_NAME)
    message(FATAL_ERROR "You must provide a name")
  endif(NOT PARSED_ARGS_NAME)

  add_pagoda_build_unit(
    TYPE                LIBRARY
    NAME                ${PARSED_ARGS_NAME}
    SOURCES             ${PARSED_ARGS_SOURCES}
    DEPENDENCIES        ${PARSED_ARGS_DEPENDENCIES}
    COMPILE_DEFINITIONS ${PARSED_ARGS_COMPILE_DEFINITIONS}
  )

  set_target_properties(${PARSED_ARGS_NAME} PROPERTIES PREFIX "")
  set_target_properties(${PARSED_ARGS_NAME} PROPERTIES OUTPUT_NAME "lib${PARSED_ARGS_NAME}")
endfunction()

# TODO: once all tests use the add_unit_test function we can remove
# PAGODA_GTEST_LIBS
set(PAGODA_GTEST_LIBS "${CONAN_LIBS_GTEST}")
# Adds a unit test executable from a single unit test source.
# The unit test name is based on the source file and assumes that
# the source file is suffixed by a .test.cpp.
# Example: unit.test.cpp -> unit
#
# param unit_test_src: the source file.
function (add_unit_test unit_test_src)
  get_filename_component(unit_test_base_name ${unit_test_src} NAME)
  string(REPLACE ".test.cpp" "_test" test_name ${unit_test_base_name})
  set(unit_test_libs "${CONAN_LIBS_GTEST}")

  add_pagoda_executable(
    NAME      ${test_name}
    SOURCES   ${unit_test_src}
    DEPENDENCIES ${unit_test_libs}
  )

  add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()
