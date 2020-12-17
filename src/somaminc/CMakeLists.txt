set( _headers checker/mincformatchecker.h
              image/mincimagereader.h
              image/mincimagereader_d.h
              image/mincimagewriter.h
              image/mincimagewriter_d.h
              plugin/mincplugin.h
)

set( _sources checker/mincformatchecker.cc
              image/mincimagereader.cc
              image/mincimagewriter.cc
              plugin/mincplugin.cc
)

add_definitions( ${LIBMINC_DEFINITIONS} )
include_directories( ${LIBMINC_INCLUDE_DIRS} )
if( MINC2_FOUND )
  set( SOMA_HAS_MINC2 "1" )
endif()

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config/somaminc_config.h.in" "${CMAKE_BINARY_DIR}/include/soma-io/config/somaminc_config.h" @ONLY )
BRAINVISA_INSTALL( FILES "${CMAKE_BINARY_DIR}/include/sona-io/config/somaminc_config.h"
                   DESTINATION "include/soma-io/config"
                   COMPONENT ${PROJECT_NAME}-dev )

BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )
set(SOMAMINC_HEADERS ${_headers} PARENT_SCOPE)
add_library( somaminc SHARED ${_sources} )
add_dependencies( somaminc ${headersTarget} )
set_property( TARGET somaminc PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( somaminc soma-io cartobase ${LIBMINC_LIBRARIES} )
BRAINVISA_INSTALL( TARGETS somaminc
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )


if( APPLE )
  # Setting VERSION or SOVERSION on a MODULE cause a link error on MacOS 10.5 with cmake 2.6:
  #  i686-apple-darwin9-g++-4.0.1: -compatibility_version only allowed with -dynamiclib
  add_library( bundle_somaminc MODULE plugin/dummy.cc )
  set_property( TARGET bundle_somaminc PROPERTY OUTPUT_NAME somaminc.${${PROJECT_NAME}_VERSION} )
  target_link_libraries( bundle_somaminc somaminc )
  BRAINVISA_INSTALL( TARGETS bundle_somaminc
                    DESTINATION lib
                    COMPONENT ${PROJECT_NAME} )
endif()

