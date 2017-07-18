#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiframeReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/MultiFrameDicomImage.h>
#include <soma-io/Dicom/MultiFrameContext.h>
#include <soma-io/Container/DicomProxy.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/MultiframeReader.h>
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



dcm::MultiframeReader::MultiframeReader()
                     : dcm::SingleFileReader()
{
}


bool dcm::MultiframeReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    Sint32 nFrames = 1;

    if ( dataset->findAndGetSint32( DCM_NumberOfFrames, nFrames ).good() )
    {

      _dataInfo->_frames = (int32_t)nFrames;

      return true;

    }

  }

  return false;

}


bool dcm::MultiframeReader::readData( dcm::DicomDatasetHeader& datasetHeader,
                                      dcm::DicomProxy& proxy)
{

  if ( proxy.allocate() )
  {

    std::string fileName = datasetHeader.getFileList().front();
    dcm::MultiFrameDicomImage* dicomImage = 
           dynamic_cast< dcm::MultiFrameDicomImage* >( getDicomImage( proxy ) );

    if ( dicomImage && dicomImage->load( fileName ) )
    {

      dcm::DataInfo& info = proxy.getDataInfo();
      int32_t startT = info._boundingBox.getLowerT();
      int32_t count = info._boundingBox.getUpperT() - startT + 1;
      float min = 0.0f, max = 0.0f;

      dcm::MultiFrameContext context( *dicomImage );
#ifdef SOMA_IO_DICOM
      carto::ThreadedLoop threadedLoop( &context, startT, count );
#else
      dcm::ThreadedLoop threadedLoop( &context, startT, count );
#endif

      threadedLoop.launch();

      dicomImage->getMinMaxValues( min, max );

      if ( min != max )
      {

        info._minimum = min;
        info._maximum = max;

      }
      else
      {

        info._minimum = 0.0;
        info._maximum = float( ( 1 << info._bitsStored ) - 1 );

      }
      
      delete dicomImage;

      return true;

    }

  }

  return false;

}


dcm::DicomImage* dcm::MultiframeReader::getDicomImage( dcm::DicomProxy& proxy )
{

  dcm::ImagePixel::Parameters parameters( proxy );

  return new dcm::MultiFrameDicomImage( proxy, parameters );

}
