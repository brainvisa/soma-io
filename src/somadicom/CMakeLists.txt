if (DCMTK_oflog_INCLUDE_DIR)
  set(SOMA_DICOM_JPEG2000_SUPPORT 1)
endif()

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config/somadicom.h.in" "${CMAKE_BINARY_DIR}/include/soma-io/config/somadicom.h" @ONLY )
BRAINVISA_INSTALL( FILES "${CMAKE_BINARY_DIR}/include/soma-io/config/somadicom.h"
                   DESTINATION "include/soma-io/config"
                   COMPONENT ${PROJECT_NAME}-dev )

set( _headers
  checker/dicomformatchecker.h
  Color/Rgba.h
  Container/BoundingBox.h
  Container/DataInfo.h
  Container/DataInfoCache.h
  Container/DicomProxy.h
  Container/Matrix.h
  Container/Vector.h
  Dicom/AcquisitionModule.h
  Dicom/Bruker/BrukerEnhancedIndexModule.h
  Dicom/Bruker/BrukerEnhancedMRReader.h
  Dicom/ColorLut.h
  Dicom/CRImageStorageReader.h
  Dicom/CTImageStorageReader.h
  Dicom/CTModule.h
  Dicom/DatasetModule.h
  Dicom/DicomDataContext.h
  Dicom/DicomDatasetHeader.h
  Dicom/DicomImage.h
  Dicom/DicomIO.h
  Dicom/DicomModule.h
  Dicom/DicomReader.h
  Dicom/DicomReaderFactory.h
  Dicom/DicomSelectContext.h
  Dicom/DicomSortContext.h
  Dicom/DiffusionModule.h
  Dicom/EnhancedIndexModule.h
  Dicom/EnhancedModalityLutModule.h
  Dicom/EnhancedMRImageStorageReader.h
  Dicom/FileInformation.h
  Dicom/GE/GEDiffusionModule.h
  Dicom/GE/GEMRReader.h
  Dicom/GenericCRReader.h
  Dicom/GenericCTReader.h
  Dicom/GenericEnhancedMRReader.h
  Dicom/GenericMRReader.h
  Dicom/GenericNMReader.h
  Dicom/GenericPTReader.h
  Dicom/GenericSCReader.h
  Dicom/GenericUSMultiframeReader.h
  Dicom/GenericUSReader.h
  Dicom/IdentifyingModule.h
  Dicom/ImageModule.h
  Dicom/ImagePixel.h
  Dicom/ModalityLutModule.h
  Dicom/MonochromeImage.h
  Dicom/MRImageStorageReader.h
  Dicom/MRModule.h
  Dicom/MultiFileReader.h
  Dicom/MultiFrameContext.h
  Dicom/MultiFrameDicomImage.h
  Dicom/MultiSliceContext.h
  Dicom/MultiSliceDicomImage.h
  Dicom/MultiSliceReader.h
  Dicom/NMImageStorageReader.h
  Dicom/NuclearModule.h
  Dicom/PaletteImage.h
  Dicom/PaletteModule.h
  Dicom/PatientModule.h
  Dicom/Philips/PhilipsDiffusionModule.h
  Dicom/Philips/PhilipsEnhancedDiffusionModule.h
  Dicom/Philips/PhilipsEnhancedIndexModule.h
  Dicom/Philips/PhilipsEnhancedMRReader.h
  Dicom/Philips/PhilipsMRReader.h
  Dicom/Philips/PhilipsNuclearModule.h
  Dicom/Philips/PhilipsPTReader.h
  Dicom/PTImageStorageReader.h
  Dicom/RadiopharmaceuticalModule.h
  Dicom/RelationshipModule.h
  Dicom/RGBImage.h
  Dicom/SCImageStorageReader.h
  Dicom/Siemens/Demosaicer.h
  Dicom/Siemens/MosaicDataContext.h
  Dicom/Siemens/MosaicDicomImage.h
  Dicom/Siemens/SiemensDiffusionModule.h
  Dicom/Siemens/SiemensMRReader.h
  Dicom/USImageStorageReader.h
  Dicom/USMultiframeImageStorageReader.h
  Dicom/USReader.h
  Dicom/USRegionSpatialModule.h
  image/dicomimagereader.h
  image/dicomimagereader_d.h
  Object/BinaryHeader.h
  Object/CartoHeader.h
  Object/HeaderProxy.h
  Pattern/Singleton.h
  plugin/dicomplugin.h
  System/DirectoryParser.h
  Transformation/AxialTransformation3d.h
  Transformation/DicomTransformation3d.h
  Transformation/PatientOrientation.h
  Transformation/Transformation3d.h
  Utils/StdInt.h
)

set( _sources
  checker/dicomformatchecker.cc
  Color/Rgba.cc
  Container/BoundingBox.cc
  Container/DataInfo.cc
  Container/DataInfoCache.cc
  Container/DicomProxy.cc
  Container/Matrix.cc
  Container/Vector.cc
  Dicom/AcquisitionModule.cc
  Dicom/Bruker/BrukerEnhancedIndexModule.cc
  Dicom/Bruker/BrukerEnhancedMRReader.cc
  Dicom/ColorLut.cc
  Dicom/CRImageStorageReader.cc
  Dicom/CTImageStorageReader.cc
  Dicom/CTModule.cc
  Dicom/DatasetModule.cc
  Dicom/DicomDataContext.cc
  Dicom/DicomDatasetHeader.cc
  Dicom/DicomImage.cc
  Dicom/DicomIO.cc
  Dicom/DicomModule.cc
  Dicom/DicomReader.cc
  Dicom/DicomReaderFactory.cc
  Dicom/DicomSelectContext.cc
  Dicom/DicomSortContext.cc
  Dicom/DiffusionModule.cc
  Dicom/EnhancedIndexModule.cc
  Dicom/EnhancedModalityLutModule.cc
  Dicom/EnhancedMRImageStorageReader.cc
  Dicom/FileInformation.cc
  Dicom/GE/GEDiffusionModule.cc
  Dicom/GE/GEMRReader.cc
  Dicom/GenericCRReader.cc
  Dicom/GenericCTReader.cc
  Dicom/GenericEnhancedMRReader.cc
  Dicom/GenericMRReader.cc
  Dicom/GenericNMReader.cc
  Dicom/GenericPTReader.cc
  Dicom/GenericSCReader.cc
  Dicom/GenericUSMultiframeReader.cc
  Dicom/GenericUSReader.cc
  Dicom/IdentifyingModule.cc
  Dicom/ImageModule.cc
  Dicom/ImagePixel.cc
  Dicom/ModalityLutModule.cc
  Dicom/MonochromeImage.cc
  Dicom/MRImageStorageReader.cc
  Dicom/MRModule.cc
  Dicom/MultiFileReader.cc
  Dicom/MultiFrameContext.cc
  Dicom/MultiFrameDicomImage.cc
  Dicom/MultiSliceContext.cc
  Dicom/MultiSliceDicomImage.cc
  Dicom/MultiSliceReader.cc
  Dicom/NMImageStorageReader.cc
  Dicom/NuclearModule.cc
  Dicom/PaletteImage.cc
  Dicom/PaletteModule.cc
  Dicom/PatientModule.cc
  Dicom/Philips/PhilipsDiffusionModule.cc
  Dicom/Philips/PhilipsEnhancedDiffusionModule.cc
  Dicom/Philips/PhilipsEnhancedIndexModule.cc
  Dicom/Philips/PhilipsEnhancedMRReader.cc
  Dicom/Philips/PhilipsMRReader.cc
  Dicom/Philips/PhilipsNuclearModule.cc
  Dicom/Philips/PhilipsPTReader.cc
  Dicom/PTImageStorageReader.cc
  Dicom/RadiopharmaceuticalModule.cc
  Dicom/RelationshipModule.cc
  Dicom/RGBImage.cc
  Dicom/SCImageStorageReader.cc
  Dicom/Siemens/Demosaicer.cc
  Dicom/Siemens/MosaicDataContext.cc
  Dicom/Siemens/MosaicDicomImage.cc
  Dicom/Siemens/SiemensDiffusionModule.cc
  Dicom/Siemens/SiemensMRReader.cc
  Dicom/USImageStorageReader.cc
  Dicom/USMultiframeImageStorageReader.cc
  Dicom/USReader.cc
  Dicom/USRegionSpatialModule.cc
  image/dicomimagereader.cc
  Object/BinaryHeader.cc
  Object/CartoHeader.cc
  Object/HeaderProxy.cc
  plugin/dicomplugin.cc
  System/DirectoryParser.cc
  Transformation/AxialTransformation3d.cc
  Transformation/DicomTransformation3d.cc
  Transformation/PatientOrientation.cc
  Transformation/Transformation3d.cc
)

if (SOMA_DICOM_JPEG2000_SUPPORT)
  # Add JPEG 2000 support for Dicom
  set ( _headers
    ${_headers}
    Dicom/DcmtkJpeg2000/dj2kcodd.h
    Dicom/DcmtkJpeg2000/dj2kcode.h
    Dicom/DcmtkJpeg2000/dj2kutil.h
    Dicom/DcmtkJpeg2000/djcparam.h
    Dicom/DcmtkJpeg2000/djdecode.h
    Dicom/DcmtkJpeg2000/djencode.h
    Dicom/DcmtkJpeg2000/djrparam.h
  )

  set ( _sources 
    ${_sources}
    Dicom/DcmtkJpeg2000/dj2kcodd.cc
    Dicom/DcmtkJpeg2000/dj2kcode.cc
    Dicom/DcmtkJpeg2000/dj2kutil.cc
    Dicom/DcmtkJpeg2000/djcparam.cc
    Dicom/DcmtkJpeg2000/djdecode.cc
    Dicom/DcmtkJpeg2000/djencode.cc
    Dicom/DcmtkJpeg2000/djrparam.cc )
endif()

add_definitions( -DSOMA_IO_DICOM -DHAVE_CONFIG_H )

BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers soma-io headersTarget )
set(SOMADICOM_HEADERS ${_headers} PARENT_SCOPE)
add_library( somadicom SHARED ${_sources} )
add_dependencies( somadicom ${headersTarget} )
set_property( TARGET somadicom PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( somadicom soma-io cartobase ${DCMTK_LIBRARIES} ${JASPER_LIBRARIES} ${TIFF_LIBRARIES} ${PNG12_LIBRARIES})
BRAINVISA_INSTALL( TARGETS somadicom
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )

if( APPLE )
  # Setting VERSION or SOVERSION on a MODULE cause a link error on MacOS 10.5 with cmake 2.6:
  #  i686-apple-darwin9-g++-4.0.1: -compatibility_version only allowed with -dynamiclib
  add_library( bundle_somadicom MODULE plugin/dummy.cc )
  set_property( TARGET bundle_somadicom PROPERTY OUTPUT_NAME somadicom.${${PROJECT_NAME}_VERSION} )
  target_link_libraries( bundle_somadicom somadicom )
  BRAINVISA_INSTALL( TARGETS bundle_somadicom
                    DESTINATION lib
                    COMPONENT ${PROJECT_NAME} )
endif()
