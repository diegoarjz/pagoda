set(test_srcs
    "main.cpp"
)

add_executable(regression_tests ${test_srcs})

set_target_properties(
    regression_tests
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_features(
    regression_tests
    PRIVATE
        cxx_std_17
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    regression_tests
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_include_directories(
    regression_tests
    PUBLIC
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
)

target_compile_definitions(
    regression_tests
    PRIVATE
        -DASSERTS_ENABLED=${Selector_ENABLE_ASSERTIONS}
)

target_link_libraries(
    regression_tests
    PRIVATE
        selector
        Boost::chrono
        Boost::system
        Boost::filesystem
        gmock
        gtest
        pthread
)

file(COPY test_files DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME regression_tests COMMAND regression_tests)
