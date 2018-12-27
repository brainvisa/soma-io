## Soma-IO config
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
  checker/achecker.h
  checker/formatchecker.h
  checker/jsonformatchecker.h
  checker/pythonformatchecker.h
  checker/xmlformatchecker.h
  config/info.h
  config/soma_config.h
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
  doc/getopt_doc.h
  doc/io_doc.h
  doc/somaio_doc.h
  getopt/getopt.h
  image/imagereader.h
  image/imagereader_d.h
  image/imagewriter.h
  image/imagewriter_d.h
  io/formatdictionary.h
  io/formatdictionary_d.h
  io/reader.h
  io/reader_d.h
  io/readeralgorithm.h
  io/scaledcoding.h
  io/writer.h
  io/writer_d.h
  plugin/plugin.h
  reader/areader.h
  reader/formatreader.h
  reader/formatreader_d.h
  reader/itemreader.h
  reader/jsonformatreader.h
  reader/pythonformatreader.h
  reader/pythonreader.h
  reader/pythonreader_d.h
  reader/xmlformatreader.h
  transformation/affinetransformation3d_base.h
  transformation/transformation.h
  utilities/allocatedvector.h
  utilities/asciidatasourcetraits.h
  utilities/creator.h
  utilities/headertranslator.h
  utilities/multifileformat.h
  utilities/minfXML2.h
  vector/vector.h
  writer/awriter.h
  writer/formatwriter.h
  writer/itemwriter.h
  writer/jsonformatwriter.h
  writer/pythonformatwriter.h
  writer/pythonwriter.h
  writer/pythonwriter_d.h
)

set( _somaio_sources
  allocator/allocator.cc
  allocator/mappingcopy.cc
  allocator/mappingro.cc
  allocator/mappingrw.cc
  allocator/meminfo.cc
  checker/achecker.cc
  checker/jsonformatchecker.cc
  checker/pythonformatchecker.cc
  checker/xmlformatchecker.cc
  config/info.cc
  config/plugins.cc
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
  getopt/getopt.cc
  image/imagereader.cc
  image/imagewriter.cc
  io/formatdictionary.cc
  io/reader_object.cc
  io/readeralgorithm.cc
  io/scaledcoding.cc
  io/writer.cc
  plugin/plugin.cc
  reader/areader.cc
  reader/jsonformatreader.cc
  reader/pythonformatreader.cc
  reader/pythonreader.cc
  reader/xmlformatreader.cc
  transformation/affinetransformation3d_base.cc
  transformation/inversion_lu.cc
  transformation/transformation.cc
  utilities/asciidatasourcetraits.cc
  utilities/minfXML2.cc
  utilities/multifileformat.cc
  vector/vector2d.cc
  vector/vector3d.cc
  vector/vector4d.cc
  writer/awriter.cc
  writer/jsonformatwriter.cc
  writer/pythonformatwriter.cc
  writer/pythonwriter.cc
)

BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )

add_library( soma-io SHARED ${_somaio_sources} )
add_dependencies( soma-io ${headersTarget} )
set_property( TARGET soma-io PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( soma-io cartobase "${CMAKE_DL_LIBS}" )
BRAINVISA_INSTALL( TARGETS soma-io
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )

IF ( DOXYGEN_FOUND )
  SET(component_name "soma-io")
  set( DOXYFILE_PREDEFINED "${SOMA-IO_DEFINITIONS}")
  set(_somaio_version
    "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}")
  set( DOXYFILE_TAGFILES
    "${CMAKE_BINARY_DIR}/share/doc/cartobase-${_somaio_version}/doxygen/cartobase.tag=../../cartobase-${_somaio_version}/doxygen" )
  set( _doxyheaders ${_headers} "config/config.h" ) # add config.h file
  BRAINVISA_GENERATE_DOXYGEN_DOC(
    _doxyheaders
    INPUT_PREFIX "${CMAKE_BINARY_DIR}/include/${component_name}"
    COMPONENT "${component_name}" )
  add_dependencies( ${component_name}-doxygen cartobase-doxygen)
ENDIF ( DOXYGEN_FOUND )
