#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/CRImageStorageReader.h>
#else
#include <Dicom/CRImageStorageReader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::CRImageStorageReader::CRImageStorageReader()
                         : dcm::SingleFileReader()
{
}


std::string dcm::CRImageStorageReader::getStorageUID()
{

  return UID_ComputedRadiographyImageStorage;

}


bool dcm::CRImageStorageReader::readHeader( DcmDataset* dataset )
{

  Float64 tmpFloat;
  if ( dataset->findAndGetFloat64( DCM_SpatialResolution, tmpFloat ).good() )
  {

    _dataInfo->_resolution.x = (double)tmpFloat;
    _dataInfo->_resolution.y = (double)tmpFloat;

  }

  return dcm::SingleFileReader::readHeader( dataset );

}
