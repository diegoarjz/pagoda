# Default Selector version, build number and date
if (NOT DEFINED Selector_VERSION)
    set(Selector_VERSION "0")
endif()

if (NOT DEFINED Selector_BUILD_NUMBER)
    set(Selector_BUILD_NUMBER "0")
endif()

if (NOT DEFINED Selector_BUILD_DATE)
    set(Selector_BUILD_DATE "0/0/0")
endif()

if (DEFINED Selector_ASSERTIONS)
    set(Selector_ENABLE_ASSERTIONS 1)
else()
    set(Selector_ENABLE_ASSERTIONS 0)
endif()

message(STATUS "Selector version:      ${Selector_VERSION}")
message(STATUS "Selector build number: ${Selector_BUILD_NUMBER}")
message(STATUS "Selector build date:   ${Selector_BUILD_DATE}")
message(STATUS "Selector Assertions:   ${Selector_ENABLE_ASSERTIONS}")
