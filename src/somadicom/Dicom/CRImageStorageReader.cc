#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/CRImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/CRImageStorageReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::CRImageStorageReader::CRImageStorageReader()
                          : soma::MultiFileReader()
{
}


std::string soma::CRImageStorageReader::getStorageUID()
{

  return UID_ComputedRadiographyImageStorage;

}


bool soma::CRImageStorageReader::readHeader( DcmDataset* dataset )
{

  Float64 tmpFloat;
  if ( dataset->findAndGetFloat64( DCM_SpatialResolution, tmpFloat ).good() )
  {

    _dataInfo->_resolution.x = (double)tmpFloat;
    _dataInfo->_resolution.y = (double)tmpFloat;

  }

  return soma::MultiFileReader::readHeader( dataset );

}


bool soma::CRImageStorageReader::readData( 
                                        soma::DicomDatasetHeader& datasetHeader,
                                        soma::DicomProxy& proxy )
{

  return soma::MultiFileReader::readData( datasetHeader, 
                                          proxy, 
                                          false );

}
