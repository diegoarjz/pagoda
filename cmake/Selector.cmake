# Default Selector version, build number and date
if (NOT DEFINED SELECTOR_VERSION)
    set(SELECTOR_VERSION "developer-version")
endif()

if (NOT DEFINED SELECTOR_BUILD_NUMBER)
    set(SELECTOR_BUILD_NUMBER "")
endif()

string(TIMESTAMP SELECTOR_BUILD_DATE "%d/%m/%Y %H:%M:%S")

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

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    option(SELECTOR_PROFILER_ACTIVE "Enable or disable built in profiling" OFF)
    option(SELECTOR_ENABLE_ASSERTIONS "Enable or disable assertions" OFF)
else()
    option(SELECTOR_PROFILER_ACTIVE "Enable or disable built in profiling" ON)
    option(SELECTOR_ENABLE_ASSERTIONS "Enable or disable assertions" ON)
endif()

string(REPLACE "\"" "" GIT_COMMIT_SUBJECT "${GIT_COMMIT_SUBJECT}")
string(REPLACE "\"" "" GIT_COMMIT_AUTHOR "${GIT_COMMIT_AUTHOR}")
string(REPLACE "\"" "" GIT_BRANCH "${GIT_BRANCH}")
set(GIT_COMMIT_SUBJECT "\"${GIT_COMMIT_SUBJECT}\"")
set(GIT_COMMIT_AUTHOR "\"${GIT_COMMIT_AUTHOR}\"")

set(SELECTOR_INCLUDE_GIT_INFO 0)
if ("${GIT_BRANCH}" STREQUAL "master")
    option(SELECTOR_GIT_INFO "Include git information in the version information" OFF)
else()
    option(SELECTOR_GIT_INFO "Include git information in the version information" ON)
endif()

message(STATUS "Version:            ${SELECTOR_VERSION}")
message(STATUS "Build number:       ${SELECTOR_BUILD_NUMBER}")
message(STATUS "Build date:         ${SELECTOR_BUILD_DATE}")
message(STATUS "Profiler Active:    ${SELECTOR_PROFILER_ACTIVE}")
message(STATUS "Assertions Active:  ${SELECTOR_ENABLE_ASSERTIONS}")
if (${SELECTOR_GIT_INFO})
    message(STATUS "Git Branch:         ${GIT_BRANCH}")
    message(STATUS "Git Commit Hash:    ${GIT_COMMIT_HASH}")
    message(STATUS "Git Commit Author:  ${GIT_COMMIT_AUTHOR}")
    message(STATUS "Git Commit Subject: ${GIT_COMMIT_SUBJECT}")
    set(SELECTOR_INCLUDE_GIT_INFO 1)
endif()

