set( _headers
  checker/niftiformatchecker.h
  checker/niftistructwrapper.h
#   checker/transformation.h
  image/niftiimagereader.h
  image/niftiimagereader_d.h
  image/niftiimagewriter.h
  image/niftiimagewriter_d.h
  nifticlib/niftiapihelpers_p.h
  nifticlib/znzlib/znzlib.h
  nifticlib/niftilib/niftilib_defines.h
  nifticlib/niftilib/nifti1.h
  nifticlib/niftilib/nifti1_io.h
  nifticlib/niftilib/nifti2.h
  nifticlib/niftilib/nifti2_io.h
  plugin/niftiplugin.h
)

set( _sources
  checker/niftiformatchecker.cc
#   checker/transformation.cc
#   checker/inversion_lu.cc
  image/niftiimagereader.cc
  image/niftiimagewriter.cc
  nifticlib/niftiapihelpers_p.cc
  nifticlib/znzlib/znzlib.c
#   nifticlib/niftilib/nifti1_io.c ## included in nifti2_io.c
  nifticlib/niftilib/nifti2_io.cc
  plugin/niftiplugin.cc
)

add_definitions( "-DHAVE_ZLIB" )
include_directories( "${CMAKE_BINARY_DIR}/include/soma-io/nifticlib/znzlib" "${CMAKE_BINARY_DIR}/include/soma-io/nifticlib/niftilib" )

BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )
set(SOMANIFTI_HEADERS ${_headers} PARENT_SCOPE)
add_library( somanifti SHARED ${_sources} )
add_dependencies( somanifti ${headersTarget} )
set_property( TARGET somanifti PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( somanifti soma-io cartobase ${ZLIB_LIBRARIES} )
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
