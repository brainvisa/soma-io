#include <soma-io/Dicom/USImageStorageReader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
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


bool soma::USImageStorageReader::readData( soma::DicomProxy& proxy,
                                           soma::Callback* progress )
{

  if ( proxy.allocate() )
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

        soma::DataInfo& info = proxy.getDataInfo();
        int32_t bpp = info.m_bpp;
        uint32_t sliceSize = info.m_sliceSize;
        uint8_t* pIn = (uint8_t*)dcmImage.getInterData()->getData();
        int32_t representation = dcmImage.getInterData()->getRepresentation();

        if ( ( bpp == 2 ) && ( representation < 2 ) )
        {

          if ( proxy.isMemoryMapped() )
          {

            int32_t sizeX = info.m_width;
            int32_t sizeY = info.m_height;
            int32_t x, y;

            for ( y = 0; y < sizeY; y++ )
            {

              for ( x = 0; x < sizeX; x++ )
              {

                *( (int16_t*)proxy( x, y ) ) = int16_t( *pIn++ );

              }

            }

          }
          else
          {

            uint16_t* p = (uint16_t*)proxy( 0 );

            while ( sliceSize-- )
            {

              *p++ = (uint16_t)*pIn++;

            }

          }

        }
        else
        {

          if ( proxy.isMemoryMapped() )
          {

            int32_t sizeX = info.m_width;
            int32_t sizeY = info.m_height;
            int32_t x, y;

            for ( y = 0; y < sizeY; y++ )
            {

              for ( x = 0; x < sizeX; x++, pIn += bpp )
              {

                std::memcpy( proxy( x, y ), pIn, bpp );

              }

            }

          }
          else
          {

            std::memcpy( proxy( 0 ), pIn, sliceSize * bpp );

          }

        }

        info.m_pixelRepresentation = representation;

        if ( bpp < 3 )
        {

          double min = 0.0, max = 0.0;

          dcmImage.getMinMaxValues( min, max );
          info.m_minimum = int32_t( min );
          info.m_maximum = int32_t( max );

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
