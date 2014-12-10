#include <soma-io/Dicom/USMultiframeImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/MultiFrameContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
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

      m_dataInfo->m_frames = (int32_t)nFrames;

    }

    return USReader::readHeader( dataset );

  }

  return false;

}


bool soma::USMultiframeImageStorageReader::readData( soma::DicomProxy& proxy,
                                                     soma::Callback* progress )
{

  if ( proxy.allocate() )
  {

    if ( progress )
    {

      progress->execute( 2 );

    }

    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( m_slices[ 0 ].c_str() ).good() )
    {

      if ( progress )
      {

        progress->execute( 6 );

      }

      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, dataset->getOriginalXfer() );

      if ( dcmImage.getStatus() == EIS_Normal )
      {

        soma::DataInfo& info = proxy.getDataInfo();
        info.m_pixelRepresentation = 
                                   dcmImage.getInterData()->getRepresentation();
 
        soma::MultiFrameContext context( dcmImage, proxy, false, progress );
        carto::ThreadedLoop threadedLoop( &context, 0, info.m_frames );

        threadedLoop.launch();

        if ( info.m_bpp < 3 )
        {

          double min = 0.0, max = 0.0;

          dcmImage.getMinMaxValues( min, max );
          info.m_minimum = int32_t( min );
          info.m_maximum = int32_t( max );

        }

        soma::DicomDatasetHeader datasetHeader( proxy );
        datasetHeader.add( dataset, 0 );

        if ( progress )
        {

          progress->execute( 100 );

        }

        return true;

      }

    }

  }

  return false;

}
