# BRAINVISA_GET_FILE_LIST_FROM_PRO( ${CMAKE_CURRENT_SOURCE_DIR}/dicom.pro "HEADERS" _headers "SOURCES" _sources )

set( _headers
  Container/Data.h
  Container/DataInfo.h
  Container/DatasetHeader.h
  Container/Matrix.h
  Container/Vector.h
  Dicom/CTImageStorageReader.h
  Dicom/DcmtkJpeg2000/dj2kcodd.h
  Dicom/DcmtkJpeg2000/dj2kcode.h
  Dicom/DcmtkJpeg2000/dj2kutil.h
  Dicom/DcmtkJpeg2000/djcparam.h
  Dicom/DcmtkJpeg2000/djdecode.h
  Dicom/DcmtkJpeg2000/djencode.h
  Dicom/DcmtkJpeg2000/djrparam.h
  Dicom/DicomDataContext.h
  Dicom/DicomDatasetHeader.h
  Dicom/DicomIO.h
  Dicom/DicomReaderFactory.h
  Dicom/DicomReader.h
  Dicom/DicomSortContext.h
  Dicom/EnhancedMRImageStorageReader.h
  Dicom/GenericCTReader.h
  Dicom/GenericEnhancedMRReader.h
  Dicom/GenericMRReader.h
  Dicom/GenericNMReader.h
  Dicom/GenericPTReader.h
  Dicom/GenericUSMultiframeReader.h
  Dicom/GenericUSReader.h
  Dicom/MRImageStorageReader.h
  Dicom/MultiFileReader.h
  Dicom/MultiFrameContext.h
  Dicom/MultiSliceContext.h
  Dicom/MultiSliceReader.h
  Dicom/NMImageStorageReader.h
  Dicom/PTImageStorageReader.h
  Dicom/Siemens/Demosaicer.h
  Dicom/Siemens/MosaicDataContext.h
  Dicom/Siemens/SiemensMRReader.h
  Dicom/soma_osconfig.h
  Dicom/SortInformation.h
  Dicom/USImageStorageReader.h
  Dicom/USMultiframeImageStorageReader.h
  Dicom/USReader.h
  Pattern/Callback.h
  Pattern/Singleton.h
  plugin/dicomplugin.h
  System/Directory.h
  Transformation/AxialTransformation3d.h
  Transformation/DicomTransformation3d.h
  Transformation/PatientOrientation.h
  Transformation/Transformation3d.h
  Utils/StdInt.h
)

set( _sources
  Container/Data.cc
  Container/DataInfo.cc
  Container/DatasetHeader.cc
  Container/Matrix.cc
  Container/Vector.cc
  Dicom/CTImageStorageReader.cc
  Dicom/DicomDataContext.cc
  Dicom/DicomDatasetHeader.cc
  Dicom/DicomIO.cc
  Dicom/DicomReader.cc
  Dicom/DicomReaderFactory.cc
  Dicom/DicomSortContext.cc
  Dicom/DcmtkJpeg2000/dj2kcodd.cc
  Dicom/DcmtkJpeg2000/dj2kcode.cc
  Dicom/DcmtkJpeg2000/dj2kutil.cc
  Dicom/DcmtkJpeg2000/djcparam.cc
  Dicom/DcmtkJpeg2000/djdecode.cc
  Dicom/DcmtkJpeg2000/djencode.cc
  Dicom/DcmtkJpeg2000/djrparam.cc
  Dicom/Siemens/Demosaicer.cc
  Dicom/Siemens/MosaicDataContext.cc
  Dicom/Siemens/SiemensMRReader.cc
  Dicom/EnhancedMRImageStorageReader.cc
  Dicom/GenericCTReader.cc
  Dicom/GenericEnhancedMRReader.cc
  Dicom/GenericMRReader.cc
  Dicom/GenericNMReader.cc
  Dicom/GenericPTReader.cc
  Dicom/GenericUSMultiframeReader.cc
  Dicom/GenericUSReader.cc
  Dicom/MRImageStorageReader.cc
  Dicom/MultiFileReader.cc
  Dicom/MultiFrameContext.cc
  Dicom/MultiSliceContext.cc
  Dicom/MultiSliceReader.cc
  Dicom/NMImageStorageReader.cc
  Dicom/PTImageStorageReader.cc
  Dicom/Siemens/Demosaicer.cc
  Dicom/Siemens/MosaicDataContext.cc
  Dicom/Siemens/SiemensMRReader.cc
  Dicom/SortInformation.cc
  Dicom/USImageStorageReader.cc
  Dicom/USMultiframeImageStorageReader.cc
  Dicom/USReader.cc
  Pattern/Callback.cc
  plugin/dicomplugin.cc
  System/Directory.cc
  Transformation/AxialTransformation3d.cc
  Transformation/DicomTransformation3d.cc
  Transformation/PatientOrientation.cc
  Transformation/Transformation3d.cc
)


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
