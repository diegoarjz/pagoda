set(PARAMETER_SRCS
    "variable.cpp"
    "variable.h"
    "variable_resolver.cpp"
    "variable_resolver.h"
)

set(PARAMETER_PUBLIC_HEADERS
    "variable.h"
    "variable_resolver.h"
)

add_library(parameter OBJECT ${PARAMETER_SRCS})

target_include_directories(
    parameter
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    parameter
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    parameter
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    parameter
    PRIVATE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    parameter
    PRIVATE
        common
)
