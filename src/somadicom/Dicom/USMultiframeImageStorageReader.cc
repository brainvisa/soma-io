#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/USMultiframeImageStorageReader.h>
#else
#include <Dicom/USMultiframeImageStorageReader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcuid.h>



dcm::USMultiframeImageStorageReader::USMultiframeImageStorageReader()
                                   : dcm::MultiframeReader(),
                                     dcm::USReader()
{
}


std::string dcm::USMultiframeImageStorageReader::getStorageUID()
{

  return UID_UltrasoundMultiframeImageStorage;

}


bool dcm::USMultiframeImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    if ( dcm::MultiframeReader::readHeader( dataset ) )
    {

      return dcm::USReader::readHeader( dataset, *_dataInfo );

    }

  }

  return false;

}
