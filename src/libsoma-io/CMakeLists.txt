BRAINVISA_GET_FILE_LIST_FROM_PRO(  ${CMAKE_CURRENT_SOURCE_DIR}/libsomaio.pro "HEADERS" _headers "SOURCES" _sources )
BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers "" headersTarget )
add_library( somaio SHARED ${_sources} )
add_dependencies( somaio ${headersTarget} )
set_property( TARGET somaio PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
BRAINVISA_INSTALL( TARGETS somaio
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )
