configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config/osversion.h.in" "${CMAKE_BINARY_DIR}/include/soma-io/config/osversion.h" @ONLY )
BRAINVISA_INSTALL( FILES "${CMAKE_BINARY_DIR}/include/soma-io/config/osversion.h"
                   DESTINATION "include/soma-io/config"
                   COMPONENT ${PROJECT_NAME}-dev )
                   
BRAINVISA_GET_FILE_LIST_FROM_PRO(  ${CMAKE_CURRENT_SOURCE_DIR}/openslide.pro "HEADERS" _headers "SOURCES" _sources )
BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )
set(SOMAOPENSLIDE_HEADERS ${_headers} PARENT_SCOPE)
add_library( somaopenslide SHARED ${_sources} )
add_dependencies( somaopenslide ${headersTarget} )
include_directories( ${OPENSLIDE_INCLUDE_DIRS} )
set_property( TARGET somaopenslide PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( somaopenslide soma-io cartobase ${OPENSLIDE_LIBRARIES}  )
BRAINVISA_INSTALL( TARGETS somaopenslide
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )
