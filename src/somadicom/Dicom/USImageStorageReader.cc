#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/USImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomImage.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/USImageStorageReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/DicomImage.h>
#include <Container/DicomProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::USImageStorageReader::USImageStorageReader()
                          : soma::USReader()
{
}


std::string soma::USImageStorageReader::getStorageUID()
{

  return UID_UltrasoundImageStorage;

}


bool soma::USImageStorageReader::readData( 
                                        soma::DicomDatasetHeader& datasetHeader,
                                        soma::DicomProxy& proxy )
{

  if ( proxy.allocate() )
  {

    std::string fileName = datasetHeader.getFileList().front();
    soma::ImagePixel::Parameters parameters( proxy );
    soma::DicomImage dicomImage( proxy, parameters );

    if ( dicomImage.load( fileName ) )
    {

      soma::DataInfo& info = proxy.getDataInfo();
      int32_t min = 0, max = 0;

      dicomImage.fill( 0, 0 );
      dicomImage.getMinMaxValues( min, max );

      if ( min != max )
      {

        info._minimum = int32_t( min );
        info._maximum = int32_t( max );

      }
      else
      {

        info._minimum = 0;
        info._maximum = ( 1 << info._bitsStored ) - 1;

      }

      return true;

    }

  }

  return false;

}
