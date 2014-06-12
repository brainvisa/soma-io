BRAINVISA_GET_FILE_LIST_FROM_PRO(  ${CMAKE_CURRENT_SOURCE_DIR}/gis.pro "HEADERS" _headers "SOURCES" _sources )
BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )
set(SOMAGIS_HEADERS ${_headers} PARENT_SCOPE)
add_library( somagis SHARED ${_sources} )
add_dependencies( somagis ${headersTarget} )
set_property( TARGET somagis PROPERTY OUTPUT_NAME somagis.${${PROJECT_NAME}_VERSION} )
target_link_libraries( somagis soma-io cartobase )
BRAINVISA_INSTALL( TARGETS somagis
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )


if( APPLE )
  # Setting VERSION or SOVERSION on a MODULE cause a link error on MacOS 10.5 with cmake 2.6:
  #  i686-apple-darwin9-g++-4.0.1: -compatibility_version only allowed with -dynamiclib
  add_library( bundle_somagis MODULE plugin/dummy.cc )
  set_property( TARGET bundle_somagis PROPERTY OUTPUT_NAME somagis.${${PROJECT_NAME}_VERSION} )
  target_link_libraries( bundle_somagis somagis )
  BRAINVISA_INSTALL( TARGETS bundle_somagis
                    DESTINATION lib
                    COMPONENT ${PROJECT_NAME} )
endif()
