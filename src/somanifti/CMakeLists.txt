set( _headers
  checker/nifti1formatchecker.h
  checker/nifti1structwrapper.h
  checker/transformation.h
  image/nifti1imagereader.h
  image/nifti1imagereader_d.h
#   image/nifti1imagewriter.h
#   image/nifti1imagewriter_d.h
  nifticlib/znzlib/znzlib.h
  nifticlib/niftilib/nifti1.h
  nifticlib/niftilib/nifti1_io.h
  plugin/niftiplugin.h
)

set( _sources
  checker/nifti1formatchecker.cc
  checker/transformation.cc
  checker/inversion_lu.cc
  image/nifti1imagereader.cc
#   image/nifti1imagewriter.cc
  nifticlib/znzlib/znzlib.c
  nifticlib/niftilib/nifti1_io.c
  plugin/niftiplugin.cc
)

add_definitions( "-DHAVE_ZLIB" )
include_directories( "${CMAKE_BINARY_DIR}/include/soma-io/nifticlib/znzlib" "${CMAKE_BINARY_DIR}/include/soma-io/nifticlib/niftilib" )

BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )
set(SOMANIFTI_HEADERS ${_headers} PARENT_SCOPE)
add_library( somanifti SHARED ${_sources} )
add_dependencies( somanifti ${headersTarget} )
set_property( TARGET somanifti PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( somanifti soma-io cartobase )
BRAINVISA_INSTALL( TARGETS somanifti
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )

if( APPLE )
  # Setting VERSION or SOVERSION on a MODULE cause a link error on MacOS 10.5 with cmake 2.6:
  #  i686-apple-darwin9-g++-4.0.1: -compatibility_version only allowed with -dynamiclib
  add_library( bundle_somanifti MODULE plugin/dummy.cc )
  set_property( TARGET bundle_somanifti PROPERTY OUTPUT_NAME somanifti.${${PROJECT_NAME}_VERSION} )
  target_link_libraries( bundle_somanifti somanifti )
  BRAINVISA_INSTALL( TARGETS bundle_somanifti
                    DESTINATION lib
                    COMPONENT ${PROJECT_NAME} )
endif()
