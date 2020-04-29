set(VALUE_SOURCES
    "builtin_functions.cpp"
    "builtin_functions.h"
    "script_callable_body.h"
    "script_callable_body.cpp"
)

set(VALUE_PUBLIC_HEADERS
    "builtin_functions.h"
    "script_callable_body.h"
)

add_library(selscript_value OBJECT ${VALUE_SOURCES})

target_include_directories(
    selscript_value
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    selscript_value
    PRIVATE
        cxx_std_17
)

target_compile_options(
    selscript_value
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_link_libraries(
    selscript_value
    PRIVATE
        common
)
