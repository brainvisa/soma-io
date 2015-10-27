#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/USImageStorageReader.h>
#include <soma-io/Dicom/DicomImage.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/USImageStorageReader.h>
#include <Dicom/DicomImage.h>
#include <Container/DicomProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::USImageStorageReader::USImageStorageReader()
                         : dcm::SingleFileReader(),
                           dcm::USReader()
{
}


std::string dcm::USImageStorageReader::getStorageUID()
{

  return UID_UltrasoundImageStorage;

}


bool dcm::USImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    if ( dcm::USReader::readHeader( dataset, *_dataInfo ) )
    {

      return dcm::SingleFileReader::readHeader( dataset );

    }

  }

  return false;

}
