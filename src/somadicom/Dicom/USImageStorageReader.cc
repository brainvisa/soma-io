#include <Dicom/USImageStorageReader.h>
#include <Container/Data.h>
#include <Pattern/Callback.h>
#include <Utils/StdInt.h>

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::USImageStorageReader::USImageStorageReader()
                          : soma::USReader()
{
}


std::string soma::USImageStorageReader::getStorageUID()
{

  return UID_UltrasoundImageStorage;

}


bool soma::USImageStorageReader::readData( soma::Data& data,
                                           soma::Callback* progress )
{

  if ( data.Create( m_dataInfo ) )
  {

    if ( progress )
    {

      progress->execute( 10 );

    }

    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( m_slices[ 0 ].c_str() ).good() )
    {

      if ( progress )
      {

        progress->execute( 30 );

      }

      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, dataset->getOriginalXfer() );

      if ( dcmImage.getStatus() == EIS_Normal )
      {

        int32_t bpp = data.m_info.m_bpp;
        uint32_t sliceSize = data.m_info.m_sliceSize;
        uint8_t* pIn = (uint8_t*)dcmImage.getInterData()->getData();
        int32_t representation = dcmImage.getInterData()->getRepresentation();

        if ( ( bpp == 2 ) && ( representation < 2 ) )
        {

          uint16_t* p = (uint16_t*)data.getSlice( 0 );

          while ( sliceSize-- )
          {

            *p++ = (uint16_t)*pIn++;

          }

        }
        else
        {

          std::memcpy( (void*)data.getSlice( 0 ), pIn, sliceSize * bpp );

        }

        data.m_info.m_pixelRepresentation = representation & 1;

        if ( data.m_info.m_bpp < 3 )
        {

          double min = 0.0, max = 0.0;

          dcmImage.getMinMaxValues( min, max );
          data.m_info.m_minimum = int32_t( min );
          data.m_info.m_maximum = int32_t( max );

        }

        if ( progress )
        {

          progress->execute( 70 );

        }

        return true;

      }

    }

  }

  return false;

}
