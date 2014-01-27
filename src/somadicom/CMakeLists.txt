BRAINVISA_GET_FILE_LIST_FROM_PRO( ${CMAKE_CURRENT_SOURCE_DIR}/dicom.pro "HEADERS" _headers "SOURCES" _sources )
BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )
set(SOMADICOM_HEADERS ${_headers} PARENT_SCOPE)
add_library( somadicom MODULE ${_sources} )
add_dependencies( somadicom ${headersTarget} )
if( NOT APPLE )
  # Setting VERSION or SOVERSION on a MODULE cause a link error on MacOS 10.5 with cmake 2.6:
  #  i686-apple-darwin9-g++-4.0.1: -compatibility_version only allowed with -dynamiclib
  set_property( TARGET somadicom PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
else()
  set_property( TARGET somadicom PROPERTY OUTPUT_NAME somadicom.${${PROJECT_NAME}_VERSION} )
endif()
target_link_libraries( somadicom soma-io cartobase )
BRAINVISA_INSTALL( TARGETS somadicom
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )
