#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/USMultiframeImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Dicom/MultiFrameContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/USMultiframeImageStorageReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#include <Dicom/MultiFrameContext.h>
#include <Thread/ThreadedLoop.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmdata/dcuid.h>



soma::USMultiframeImageStorageReader::USMultiframeImageStorageReader()
                                    : soma::USReader()
{
}


std::string soma::USMultiframeImageStorageReader::getStorageUID()
{

  return UID_UltrasoundMultiframeImageStorage;

}


bool soma::USMultiframeImageStorageReader::readHeader( DcmDataset* dataset )
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


bool soma::USMultiframeImageStorageReader::readData( 
                                        soma::DicomDatasetHeader& datasetHeader,
                                        soma::DicomProxy& proxy)
{

  if ( proxy.allocate() )
  {

    std::string fileName = datasetHeader.getFileList().front();
    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( fileName.c_str() ).good() )
    {

      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, dataset->getOriginalXfer() );

      if ( dcmImage.getStatus() == EIS_Normal )
      {
 
        soma::DataInfo& info = proxy.getDataInfo();
        info._pixelRepresentation = 
                                   dcmImage.getInterData()->getRepresentation();
 
        soma::MultiFrameContext context( dcmImage, proxy, false );
        soma::ThreadedLoop threadedLoop( &context, 0, info._frames );

        threadedLoop.launch();

        if ( info._bpp < 3 )
        {

          double min = 0.0, max = 0.0;

          dcmImage.getMinMaxValues( min, max );
          info._minimum = int32_t( min );
          info._maximum = int32_t( max );

        }

        return true;

      }

    }

  }

  return false;

}
