#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/USMultiframeImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/MultiFrameDicomImage.h>
#include <soma-io/Dicom/MultiFrameContext.h>
#include <soma-io/Container/DicomProxy.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/USMultiframeImageStorageReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/MultiFrameDicomImage.h>
#include <Dicom/MultiFrameContext.h>
#include <Container/DicomProxy.h>
#include <Thread/ThreadedLoop.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>



dcm::USMultiframeImageStorageReader::USMultiframeImageStorageReader()
                                   : dcm::USReader()
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

    Sint32 nFrames = 1;

    if ( dataset->findAndGetSint32( DCM_NumberOfFrames, nFrames ).good() )
    {

      _dataInfo->_frames = (int32_t)nFrames;

    }

    return USReader::readHeader( dataset );

  }

  return false;

}


bool dcm::USMultiframeImageStorageReader::readData( 
                                         dcm::DicomDatasetHeader& datasetHeader,
                                         dcm::DicomProxy& proxy)
{

  if ( proxy.allocate() )
  {

    std::string fileName = datasetHeader.getFileList().front();
    dcm::ImagePixel::Parameters parameters( proxy );
    dcm::MultiFrameDicomImage dicomImage( proxy, parameters );

    if ( dicomImage.load( fileName ) )
    {

      dcm::DataInfo& info = proxy.getDataInfo();
      int32_t startT = info._boundingBox.getLowerT();
      int32_t count = info._boundingBox.getUpperT() - startT + 1;
      int32_t min = 0, max = 0;

      dcm::MultiFrameContext context( dicomImage );
#ifdef SOMA_IO_DICOM
      carto::ThreadedLoop threadedLoop( &context, startT, count );
#else
      dcm::ThreadedLoop threadedLoop( &context, startT, count );
#endif

      threadedLoop.launch();

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
