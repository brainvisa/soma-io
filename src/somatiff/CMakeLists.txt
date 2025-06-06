include_directories( ${TIFF_INCLUDE_DIR} )

set( _headers
  checker/tiffformatchecker.h
  image/tiffimagereader.h
  image/tiffimagereader_d.h
  image/tiffimagewriter.h
  image/tiffimagewriter_d.h
  plugin/tiffplugin.h
)

set( _sources
  checker/tiffformatchecker.cc
  image/tiffimagereader.cc
  image/tiffimagewriter.cc
  plugin/tiffplugin.cc
)

BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )
set(SOMATIFF_HEADERS ${_headers} PARENT_SCOPE)
add_library( somatiff SHARED ${_sources} )
add_dependencies( somatiff ${headersTarget} )
set_property( TARGET somatiff PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )

target_link_libraries( somatiff soma-io cartobase
  ${TIFF_LIBRARIES} ${ZLIB_LIBRARIES} )
  install( TARGETS somatiff
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )

if( APPLE )
  # Setting VERSION or SOVERSION on a MODULE cause a link error on MacOS 10.5 with cmake 2.6:
  #  i686-apple-darwin9-g++-4.0.1: -compatibility_version only allowed with -dynamiclib
  add_library( bundle_somatiff MODULE plugin/dummy.cc )
  set_property( TARGET bundle_somatiff PROPERTY OUTPUT_NAME somatiff.${${PROJECT_NAME}_VERSION} )
  target_link_libraries( bundle_somatiff somatiff )
  install( TARGETS bundle_somatiff
                     DESTINATION lib
                     COMPONENT ${PROJECT_NAME} )
endif()
