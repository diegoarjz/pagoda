set(test_srcs
    "main.cpp"
)

add_executable(pgscript_tests ${test_srcs})

target_compile_features(
    pgscript_tests
    PRIVATE
        cxx_std_17
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    pgscript_tests
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_include_directories(
    pgscript_tests
    PUBLIC
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
)

target_compile_definitions(
    pgscript_tests
    PUBLIC
      ${PAGODA_COMPILER_ID}
    PRIVATE
        -DASSERTS_ENABLED=${Pagoda_ENABLE_ASSERTIONS}
)

target_link_libraries(
    pgscript_tests
    PRIVATE
        libpagoda
        Boost::chrono
        Boost::system
        Boost::filesystem
        gmock
        gtest
        pthread
)

file(COPY test_files DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME pgscript_tests COMMAND pgscript_tests)
