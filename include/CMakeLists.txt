set(PAGODA_PUBLIC_HEADERS
  "pagoda/pagoda.h"
  "pagoda/api.h"
  "pagoda/common/version.h"
  "pagoda/common/plugin.h"
)

add_pagoda_header_only(
  NAME    pagoda_public_headers
  SOURCES ${PAGODA_PUBLIC_HEADERS}
)

set_target_properties(pagoda_public_headers PROPERTIES PUBLIC_HEADER "${PAGODA_PUBLIC_HEADERS}")
include(GNUInstallDirs)
install(DIRECTORY "pagoda" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
