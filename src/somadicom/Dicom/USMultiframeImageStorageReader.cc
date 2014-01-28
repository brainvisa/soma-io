#include <soma-io/Dicom/USMultiframeImageStorageReader.h>
#include <soma-io/Container/Data.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/MultiFrameContext.h>
#include <Thread/ThreadedLoop.h>
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

      m_dataInfo.m_frames = (int32_t)nFrames;

    }

    return USReader::readHeader( dataset );

  }

  return false;

}


bool soma::USMultiframeImageStorageReader::readData( soma::Data& data,
                                                     soma::Callback* progress )
{

  if ( data.Create( m_dataInfo ) )
  {

    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( m_slices[ 0 ].c_str() ).good() )
    {

      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, dataset->getOriginalXfer() );

      if ( dcmImage.getStatus() == EIS_Normal )
      {
 
        soma::MultiFrameContext context( dcmImage, data, false, progress );
        soma::ThreadedLoop threadedLoop( &context,
                                         0,
                                         data.m_info.m_frames );

        threadedLoop.launch();

        data.m_info.m_pixelRepresentation = 
                               dcmImage.getInterData()->getRepresentation() & 1;

        if ( data.m_info.m_bpp < 3 )
        {

          double min = 0.0, max = 0.0;

          dcmImage.getMinMaxValues( min, max );
          data.m_info.m_minimum = int32_t( min );
          data.m_info.m_maximum = int32_t( max );

        }

        return true;

      }

    }

  }

  return false;

}
