set( _headers
     soma/object_stream.hpp
     soma/image_stream.hpp )
set( _sources
     soma/object_stream.cpp
     soma/image_stream.cpp )


BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers "" headersTarget )
add_library( somaio SHARED ${_sources} )
add_dependencies( somaio ${headersTarget} )
set_property( TARGET somaio PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
BRAINVISA_INSTALL( TARGETS somaio
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )

# nothing to document for the moment
# IF ( DOXYGEN_FOUND )
#   set( _doxygenInput "${_headers}" )
#   set( DOXYFILE_PREDEFINED "${SOMA-IO_DEFINITIONS}")
#   BRAINVISA_GENERATE_DOXYGEN_DOC( _doxygenInput  
#                                   INPUT_PREFIX "${CMAKE_BINARY_DIR}/include/soma"
#                                   COMPONENT soma-io )
# ENDIF ( DOXYGEN_FOUND )

