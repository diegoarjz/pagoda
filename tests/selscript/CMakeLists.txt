set(test_srcs
    "main.cpp"
)

add_executable(selscript_tests ${test_srcs})

set_target_properties(
    selscript_tests
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_features(
    selscript_tests
    PRIVATE
        cxx_std_17
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    selscript_tests
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_include_directories(
    selscript_tests
    PUBLIC
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
)

target_compile_definitions(
    selscript_tests
    PRIVATE
        -DASSERTS_ENABLED=${Selector_ENABLE_ASSERTIONS}
)

target_link_libraries(
    selscript_tests
    PRIVATE
        selector
        Boost::chrono
        Boost::system
        Boost::filesystem
        pthread
        gmock
        gtest
)

file(COPY test_files DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME selscript_tests COMMAND selscript_tests)
