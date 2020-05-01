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
string(REPLACE "\"" "" GIT_COMMIT_AUTHOR "${GIT_COMMIT_AUTHOR}")
string(REPLACE "\"" "" GIT_BRANCH "${GIT_BRANCH}")
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

