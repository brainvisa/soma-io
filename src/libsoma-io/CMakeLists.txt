set( _headers
     soma/rcobject.hpp )
set( _sources
     rcobject.cpp
      )

#+++ WIP
# set( _headers ${_headers}
#      soma/wip/image_reader.hpp
#      soma/wip/gis_format_reader.hpp
# )
# set( _sources ${_sources}
#      soma/wip/gis_format_reader.cpp
# )
# # Write initial built-in plugin features info
# execute_process( COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/soma/wip/built-in.somaplugins" "${BRAINVISA_SOMA_BUILT-IN_PLUGINS_INFO}" )
#--- WIP


BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers "" headersTarget )
add_library( somaio SHARED ${_sources} )
add_dependencies( somaio ${headersTarget} )
set_property( TARGET somaio PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
BRAINVISA_INSTALL( TARGETS somaio
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )


#+++ WIP
# Adds formats-specific values to built-in plugin features info
# get_property( libname TARGET somaio PROPERTY LOCATION )
# get_filename_component( libname "${libname}" NAME )
# file( APPEND "${BRAINVISA_SOMA_BUILT-IN_PLUGINS_INFO}" "\n# GIS image format\nsoma.formats.gis\nsoma.formats.gis reader ${libname}\nsoma.formats.gis writer ${libname}" )
#--- WIP
