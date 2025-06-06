set( _headers
  checker/mifformatchecker.h
  image/mifimagereader.h
  image/mifimagereader_d.h
  plugin/mifplugin.h
)

set( _sources
  checker/mifformatchecker.cc
  image/mifimagereader.cc
  plugin/mifplugin.cc
)

add_definitions( "-DHAVE_ZLIB" )

BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )
set(SOMAMIF_HEADERS ${_headers} PARENT_SCOPE)
add_library( somamif SHARED ${_sources} )
add_dependencies( somamif ${headersTarget} )
set_property( TARGET somamif PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( somamif soma-io cartobase ${ZLIB_LIBRARIES} )
install( TARGETS somamif
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )

if( APPLE )
  # Setting VERSION or SOVERSION on a MODULE cause a link error on MacOS 10.5 with cmake 2.6:
  #  i686-apple-darwin9-g++-4.0.1: -compatibility_version only allowed with -dynamiclib
  add_library( bundle_somamif MODULE plugin/dummy.cc )
  set_property( TARGET bundle_somamif PROPERTY OUTPUT_NAME somamif.${${PROJECT_NAME}_VERSION} )
  target_link_libraries( bundle_somamif somamif )
  install( TARGETS bundle_somamif
                    DESTINATION lib
                    COMPONENT ${PROJECT_NAME} )
endif()
