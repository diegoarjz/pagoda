set(SelSources
    sel.cpp
    )

add_executable(sel ${SelSources})

target_compile_features(
    sel
    PRIVATE
        cxx_std_17
)

target_compile_options(
    sel
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_include_directories(
    sel
    PUBLIC
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

target_compile_definitions(
    sel
    PRIVATE
)

target_link_libraries(
    sel
    PRIVATE
        selector
        Boost::chrono
        Boost::filesystem
        Boost::system
        Boost::program_options
)

include(GNUInstallDirs)
install(TARGETS sel
    EXPORT sel-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
