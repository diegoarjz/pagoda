add_subdirectory(intermediate)
add_subdirectory(value)
add_subdirectory(parser)
add_subdirectory(interpreter)

add_library(pgscript OBJECT
    $<TARGET_OBJECTS:pgscript_intermediate>
    $<TARGET_OBJECTS:pgscript_interpreter>
    $<TARGET_OBJECTS:pgscript_parser>
    $<TARGET_OBJECTS:pgscript_value>
)

target_include_directories(
    pgscript
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    pgscript
    PRIVATE
        cxx_std_17
)

target_compile_options(
    pgscript
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_link_libraries(
    pgscript
    PRIVATE
        common
)
