add_subdirectory(common)
add_subdirectory(geometry)
add_subdirectory(image)
add_subdirectory(material)
add_subdirectory(math)
add_subdirectory(scene)
add_subdirectory(objects)
add_subdirectory(graph)
add_subdirectory(dynamic)
add_subdirectory(pgscript)

set(PAGODA_SOURCES
  "pagoda.cpp"
  $<TARGET_OBJECTS:common>
  $<TARGET_OBJECTS:geometry>
  $<TARGET_OBJECTS:image>
  $<TARGET_OBJECTS:material>
  $<TARGET_OBJECTS:scene>
  $<TARGET_OBJECTS:graph>
  $<TARGET_OBJECTS:objects>
  $<TARGET_OBJECTS:pgscript>
  $<TARGET_OBJECTS:dynamic>
)

set(PAGODA_DEPENDENCIES
    Boost::chrono
    Boost::filesystem
    Boost::system
    Boost::program_options
    BZip2::BZip2
    PNG::PNG
    ZLIB::ZLIB
)

add_pagoda_library(
  NAME          libpagoda
  SOURCES       ${PAGODA_SOURCES}
  DEPENDENCIES  ${PAGODA_DEPENDENCIES} ${CMAKE_DL_LIBS}
)

