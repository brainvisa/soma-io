set( _headers checker/gisformatchecker.h
              image/gisimagereader.h
                image/gisimagereader_d.h
                image/gisimagewriter.h
                image/gisimagewriter_d.h
                plugin/gisplugin.h)

set( _sources checker/gisformatchecker.cc
              image/gisimagereader.cc
              image/gisimagewriter.cc
              plugin/gisplugin.cc)
 
BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )
set(SOMAGIS_HEADERS ${_headers} PARENT_SCOPE)
add_library( somagis SHARED ${_sources} )
add_dependencies( somagis ${headersTarget} )
set_property( TARGET somagis PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( somagis soma-io cartobase )
install( TARGETS somagis
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )


if( APPLE )
  # Setting VERSION or SOVERSION on a MODULE cause a link error on MacOS 10.5 with cmake 2.6:
  #  i686-apple-darwin9-g++-4.0.1: -compatibility_version only allowed with -dynamiclib
  add_library( bundle_somagis MODULE plugin/dummy.cc )
  set_property( TARGET bundle_somagis PROPERTY OUTPUT_NAME somagis.${${PROJECT_NAME}_VERSION} )
  target_link_libraries( bundle_somagis somagis )
  install( TARGETS bundle_somagis
                    DESTINATION lib
                    COMPONENT ${PROJECT_NAME} )
endif()
