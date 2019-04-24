set(GraphRunnerSources
    graph_reader.cpp
    )

add_executable(GraphRunner ${GraphRunnerSources})

target_compile_features(
    GraphRunner
    PRIVATE
        cxx_std_17
)

target_compile_options(
    GraphRunner
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_include_directories(
    GraphRunner
    PUBLIC
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

set_target_properties(
    GraphRunner
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_definitions(
    GraphRunner
    PRIVATE
)

target_link_libraries(
    GraphRunner
    PRIVATE
        selector
        Boost::chrono
        Boost::system
        Boost::program_options
)

