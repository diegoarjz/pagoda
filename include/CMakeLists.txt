set(PAGODA_PUBLIC_HEADERS
  "pagoda/pagoda.h"
  "pagoda/api.h"
  "pagoda/common/factory.h"
  "pagoda/common/pluggable_factory.h"
  "pagoda/common/version.h"
  "pagoda/common/plugin.h"
  "pagoda/graph/graph.h"
  "pagoda/graph/node.h"
  "pagoda/graph/node_factory.h"
  "pagoda/graph/node_set.h"
  "pagoda/objects/operation_factory.h"
  "pagoda/objects/procedural_component.h"
  "pagoda/objects/procedural_component_system.h"
  "pagoda/objects/procedural_component_system_base.h"
  "pagoda/objects/procedural_object_system.h"
  "pagoda/objects/procedural_operation.h"
  "pagoda/objects/parameter.h"
  "pagoda/objects/parameter_callback.h"
)

add_pagoda_header_only(
  NAME    pagoda_public_headers
  SOURCES ${PAGODA_PUBLIC_HEADERS}
)

set_target_properties(pagoda_public_headers PROPERTIES PUBLIC_HEADER "${PAGODA_PUBLIC_HEADERS}")
include(GNUInstallDirs)
install(DIRECTORY "pagoda" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
