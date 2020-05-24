set(PagodaSources
    pagoda.cpp
    )

add_executable(pagoda ${PagodaSources})

target_compile_features(
    pagoda
    PRIVATE
        cxx_std_17
)

target_compile_options(
    pagoda
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_include_directories(
    pagoda
    PUBLIC
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
        ${PGSCRIPT_INCLUDE_DIR}
)

target_compile_definitions(
    pagoda
    PRIVATE
)

target_link_libraries(
    pagoda
    PRIVATE
        libpagoda
        Boost::chrono
        Boost::filesystem
        Boost::system
        Boost::program_options
)

include(GNUInstallDirs)
install(TARGETS pagoda
    EXPORT pagoda-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(PgeditorSources
    pgeditor.cpp
    )

add_executable(pgeditor ${PgeditorSources})

target_compile_features(
    pgeditor
    PRIVATE
        cxx_std_17
)

target_compile_options(
    pgeditor
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_include_directories(
    pgeditor
    PUBLIC
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
        ${PGSCRIPT_INCLUDE_DIR}
)

target_compile_definitions(
    pgeditor
    PRIVATE
)

target_link_libraries(
    pgeditor
    PRIVATE
        libpagoda
        libpgeditor
        Boost::chrono
        Boost::filesystem
        Boost::system
        Boost::program_options
        glfw
        ${GLEW_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${CORE_VIDEO_LIBRARY}
)

include(GNUInstallDirs)
install(TARGETS pgeditor
    EXPORT pgeditor-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
