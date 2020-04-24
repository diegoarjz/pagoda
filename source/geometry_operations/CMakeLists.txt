set(GEOMETRY_OPERATIONS_SRCS
    "create_box.h"
    "create_rect.h"
    "create_sphere.h"
    "ear_clipping.h"
    "explode_to_faces.h"
    "extrusion.h"
    "extrusion_profile.h"
    "face_offset.h"
    "plane_splits.h"
    "triangulate.h"
)

set(GEOMETRY_OPERATIONS_PUBLIC_HEADERS
    "create_box.h"
    "ear_clipping.h"
    "explode_to_faces.h"
    "create_rect.h"
    "create_sphere.h"
    "extrusion.h"
    "extrusion_profile.h"
    "face_offset.h"
    "plane_splits.h"
    "triangulate.h"
)

add_library(geometry_operations OBJECT ${GEOMETRY_OPERATIONS_SRCS})

target_include_directories(
    geometry_operations
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

set_target_properties(
    geometry_operations
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_features(
    geometry_operations
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    geometry_operations
    PRIVATE
        -Wall
        -Werror
)

target_compile_definitions(
    geometry_operations
    PRIVATE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    geometry_operations
    PRIVATE
        common
)

include(GNUInstallDirs)
install(TARGETS geometry_operations
    EXPORT geometry_operations-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/geometry_operations
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/geometry_operations
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/geometry_operations)

install(EXPORT geometry_operations-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/cmake/
)


