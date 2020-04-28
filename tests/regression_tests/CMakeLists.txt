configure_file(
    "${PROJECT_SOURCE_DIR}/tests/regression_tests/main.cpp.in"
    "${PROJECT_BINARY_DIR}/tests/regression_tests/main.cpp"
)

set(test_srcs
    "${CMAKE_CURRENT_BINARY_DIR}/main.cpp"
)

add_executable(regression_tests ${test_srcs})

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
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
        -Wall
        -Werror
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

add_test(NAME regression_tests COMMAND regression_tests)
