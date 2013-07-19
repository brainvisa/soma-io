BRAINVISA_GET_FILE_LIST_FROM_PRO(  ${CMAKE_CURRENT_SOURCE_DIR}/gis.pro "HEADERS" _headers "SOURCES" _sources )
BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )
set(SOMAGIS_HEADERS ${_headers} PARENT_SCOPE)
add_library( somagis MODULE ${_sources} )
add_dependencies( somagis ${headersTarget} )
set_property( TARGET somagis PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( somagis ${SOMA-IO_LIBRARIES} )
BRAINVISA_INSTALL( TARGETS somagis
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )
