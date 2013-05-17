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
  allocator/allocator.h
  allocator/mappingcopy.h
  allocator/mappingro.h
  allocator/mappingrw.h
  cartodata/volume.h
  cartodata/volume_d.h
  cartodata/volumeproxy.h
  cartodata/volumeproxy_d.h
  cartodata/volumeutil.h
  cartodata/volumeutil_d.h
  cartodata/volumeoperators.h
  cartodata/volumeview.h
  cartodata/volumeformatreader.h
  cartodata/volumeformatreader_d.h
  cartodata/volumeformatwriter.h
  cartodata/volumeformatwriter_d.h
  #checker/achecker.h
  checker/formatchecker.h
  checker/pythonformatchecker.h
  checker/xmlformatchecker.h
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
  datasourceinfo/datasourceinfoloader.h
  image/imagereader.h
  image/imagereader_d.h
  image/imagewriter.h
  image/imagewriter_d.h
  io/formatdictionary.h
  io/formatdictionary_d.h
  io/reader.h
  io/reader_d.h
  #io/readeralgorithm.h
  io/writer.h
  io/writer_d.h
  plugingis/gisformatchecker.h
  plugingis/gisimagereader.h
  plugingis/gisimagereader_d.h
  plugingis/gisimagewriter.h
  plugingis/gisimagewriter_d.h
  reader/areader.h
  reader/formatreader.h
  reader/formatreader_d.h
  reader/itemreader.h
  reader/pythonformatreader.h
  reader/pythonreader.h
  reader/pythonreader_d.h
  reader/xmlformatreader.h
  utilities/allocatedvector.h
  utilities/asciidatasourcetraits.h
  utilities/creator.h
  #utilities/headertranslator.h
  utilities/minfXML2.h
  #writer/awriter.h
  writer/formatwriter.h
  writer/itemwriter.h
  writer/pythonformatwriter.h
  writer/pythonwriter.h
  writer/pythonwriter_d.h
)

BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )

set( _somaio_sources
  allocator/allocator.cc
  allocator/mappingcopy.cc
  allocator/mappingro.cc
  allocator/mappingrw.cc
  allocator/meminfo.cc
  cartodata/volume.cc
  cartodata/volumeview.cc
  cartodata/volumeformatreader.cc
  cartodata/volumeformatreader_gis.cc
  cartodata/volumeformatwriter.cc
  cartodata/volumeformatwriter_gis.cc
  cartodata/reader_volume.cc
  cartodata/writer_volume.cc
  #checker/achecker.cc
  checker/pythonformatchecker.cc
  checker/xmlformatchecker.cc
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
  datasourceinfo/datasourceinfoloader.cc
  image/imagereader.cc
  image/imagewriter.cc
  io/formatdictionary.cc
  io/reader_object.cc
  #io/readeralgorithm.cc
  io/writer.cc
  plugingis/gisformatchecker.cc
  plugingis/gisimagereader.cc
  plugingis/gisimagewriter.cc
  reader/areader.cc
  reader/pythonformatreader.cc
  reader/pythonreader.cc
  reader/xmlformatreader.cc
  utilities/asciidatasourcetraits.cc
  utilities/minfXML2.cc
  #writer/awriter.cc
  writer/pythonformatwriter.cc
  writer/pythonwriter.cc
)

add_library( soma-io SHARED ${_somaio_sources} )
add_dependencies( soma-io ${headersTarget} )
set_property( TARGET soma-io PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( soma-io ${SOMA_CARTOBASE_LIB} ${LIBXML2_LIBRARIES} ${Sigc++2_LIBRARIES}  )
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

IF ( DOXYGEN_FOUND )
  SET(component_name "soma-io")
  set( DOXYFILE_PREDEFINED "${AIMS_DEFINITIONS}")
  BRAINVISA_GENERATE_DOXYGEN_DOC( _headers 
                                  INPUT_PREFIX "${CMAKE_BINARY_DIR}/include/${component_name}"
                                  COMPONENT "${component_name}")
ENDIF ( DOXYGEN_FOUND )
