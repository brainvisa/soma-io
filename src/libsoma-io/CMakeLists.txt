set( _headers
     soma/type_descriptor.hpp
     soma/io_request.hpp 
     soma/object_stream.hpp
     soma/image_stream.hpp
     soma/memory_layout.hpp
)
set( _sources
     soma/type_descriptor.cpp
     soma/io_request.cpp 
     soma/object_stream.cpp
     soma/image_stream.cpp )


BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers "" headersTarget )
add_library( somaio SHARED ${_sources} )
add_dependencies( somaio ${headersTarget} )
set_property( TARGET somaio PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
# BRAINVISA_INSTALL( TARGETS somaio
#                    DESTINATION lib
#                    COMPONENT ${PROJECT_NAME} )
