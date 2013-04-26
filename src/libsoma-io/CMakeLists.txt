# Cartobase/Soma-IO config
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config/version.h.in" "${CMAKE_BINARY_DIR}/include/soma-io/config/version.h" @ONLY )
BRAINVISA_INSTALL( FILES "${CMAKE_BINARY_DIR}/include/soma-io/config/version.h" 
                   DESTINATION "include/soma-io/config" 
                   COMPONENT ${PROJECT_NAME}-dev )
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config/config.h.in" "${CMAKE_BINARY_DIR}/include/soma-io/config/config.h" @ONLY )
BRAINVISA_INSTALL( FILES "${CMAKE_BINARY_DIR}/include/soma-io/config/config.h"
                   DESTINATION "include/soma-io/config"
                   COMPONENT ${PROJECT_NAME}-dev )

set( _headers
  #checker/formatchecker.h
  #checker/pythonformatchecker.h
  #checker/xmlformatchecker.h
  datasource/bufferdatasource.h
  datasource/chaindatasource.h
  datasource/cstreamdatasource.h
  datasource/datasource.h
  datasource/datasourcelist.h
  datasource/fddatasource.h
  datasource/filedatasource.h
  datasource/filteringdatasource.h
  datasource/streamdatasource.h
  datasourceinfo/datasourcecapabilities.h
  datasourceinfo/datasourceinfo.h
  #datasourceinfo/datasourceinfoloader.h
  #image/imagereader.h
  #imagereader_d.h
  #imagwriter.h
  io/formatdictionary.h
  io/formatdictionary_d.h
  #io/reader.h
  #io/reader_d.h
  #io/readeralgorithm.h
  #io/writer.h
  #io/writer_d.h
  #other/asciidatasourcetraits.h
  #other/creator.h
  #other/headertranslator.h
  #other/minfXML2.h
  #reader/formatreader.h
  #reader/formatreader_d.h
  #reader/itemreader.h
  #reader/pythonformatreader.h
  #reader/xmlformatreader.h
  #writer/formatwriter.h
  #writer/itrmwriter.h
  #writer/pythonformatwriter.h
)

BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )

set( _somaio_0_sources
  #checker/pythonformatchecker.cc
  #checker/xmlformatchecker.cc
  config/version.cc
  datasource/bufferdatasource.cc
  datasource/chaindatasource.cc
  datasource/cstreamdatasource.cc
  datasource/datasource.cc
  datasource/datasourcelist.cc
  datasource/fddatasource.cc
  datasource/filedatasource.cc
  datasource/filteringdatasource.cc
  datasource/streamdatasource.cc
  datasourceinfo/datasourcecapabilities.cc
  datasourceinfo/datasourceinfo.cc
  #datasourceinfo/datasourceinfoloader.cc
  io/formatdictionary.cc
  #io/reader_object.cc
  #io/readeralgorithm.cc
  #io/writer.cc
  #other/asciidatasourcetraits.cc
  #other/minfXML2.cc
  #reader/pythonformatreader.cc
  #reader/xmlformatreader.cc
  #writer/pythonformatwriter.cc
)

# set( _somaio_1_sources
# )
# 
# set( _somaio_sources
# )

add_library( soma-io SHARED ${_somaio_0_sources} )
add_dependencies( soma-io ${headersTarget} )
set_property( TARGET soma-io PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( soma-io ${LIBXML2_LIBRARIES} ${Sigc++2_LIBRARIES}  )
if( WIN32 AND CMAKE_CROSSCOMPILING )
  target_link_libraries( soma-io ws2_32 ole32 )
endif( WIN32 AND CMAKE_CROSSCOMPILING )
if( WIN32 )
  target_link_libraries( soma-io wsock32 )
endif( WIN32 )
if(UNIX)
  target_link_libraries( soma-io "m" "pthread" )
endif(UNIX)
if(APPLE)
  target_link_libraries( soma-io "-framework CoreServices" )
endif(APPLE)
BRAINVISA_INSTALL( TARGETS soma-io
              DESTINATION lib
              COMPONENT ${PROJECT_NAME} )


# add_library( soma-io_1 SHARED ${_somaio_1_sources} )
# set_property( TARGET soma-io_1 PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
# target_link_libraries( soma-io_1 "somaio_0" ${LIBXML2_LIBRARIES} ${Sigc++2_LIBRARIES} )
# if(UNIX)
#   target_link_libraries( soma-io_1 "pthread" )
# endif(UNIX)
# BRAINVISA_INSTALL( TARGETS soma-io_1
#               DESTINATION lib
#               COMPONENT ${PROJECT_NAME} )
# 
#               
# add_library( soma-io SHARED ${_somaio_sources} )
# set_property( TARGET soma-io PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
# target_link_libraries( soma-io "somaio_1" "somaio_0" ${LIBXML2_LIBRARIES} ${Sigc++2_LIBRARIES} )
# if(UNIX)
#   target_link_libraries( soma-io "pthread" )
# endif(UNIX)
# BRAINVISA_INSTALL( TARGETS soma-io
#               DESTINATION lib
#               COMPONENT ${PROJECT_NAME} )

              
IF ( DOXYGEN_FOUND )
  SET(component_name "soma-io")
  set( DOXYFILE_PREDEFINED "${AIMS_DEFINITIONS}")
  BRAINVISA_GENERATE_DOXYGEN_DOC( _headers 
                                  INPUT_PREFIX "${CMAKE_BINARY_DIR}/include/${component_name}"
                                  COMPONENT "${component_name}")
ENDIF ( DOXYGEN_FOUND )
