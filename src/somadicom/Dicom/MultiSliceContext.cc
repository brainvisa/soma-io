#include <soma-io/Dicom/MultiSliceContext.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Pattern/Callback.h>


soma::MultiSliceContext::MultiSliceContext( 
                                   DicomImage& dcmImage, 
                                   soma::DicomProxy& proxy,
                                   soma::Callback* progress )
                       : carto::LoopContext(),
                         m_dcmImage( dcmImage ),
                         m_proxy( proxy ),
                         m_progress( progress ),
                         m_count( 0 )
{
}


void soma::MultiSliceContext::doIt( int32_t startIndex, int32_t count )
{

  soma::DataInfo& info = m_proxy.getDataInfo();
  int32_t i, stopIndex = startIndex + count;
  int32_t px, py, pz, sizeX, sizeY, sizeZ;
  int32_t bpp = info.m_bpp;

  info.m_patientOrientation.getOnDiskSize( sizeX, sizeY, sizeZ );

  int32_t n = sizeZ - 1;
  uint32_t sliceSize = sizeX * sizeY;

  for ( i = startIndex; i < stopIndex; i++ )
  {

    int32_t z = i % sizeZ;
    int32_t t = i / sizeZ;

    if ( bpp == 3 )
    {

      int32_t x, y;
      int32_t offset = i * sliceSize;
      uint8_t** d = (uint8_t**)m_dcmImage.getInterData()->getData();
      uint8_t* r = d[ 0 ] + offset;
      uint8_t* g = d[ 1 ] + offset;
      uint8_t* b = d[ 2 ] + offset;

      if ( info.m_noFlip )
      {

        if ( m_proxy.isMemoryMapped() )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              uint8_t* pOut = m_proxy( x, y, z, t );

              *pOut++ = *r++;
              *pOut++ = *g++;
              *pOut = *b++;

            }

          }

        }
        else
        {

          uint32_t l = sliceSize;
          uint8_t* p = m_proxy( 0, 0, z, t );

          while ( l-- )
          {

            *p++ = *r++;
            *p++ = *g++;
            *p++ = *b++;

          }

        }

      }
      else
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            info.m_patientOrientation.getDirect( x, y, z, px, py, pz );

            uint8_t* pOut = m_proxy( px, py, pz, t );

            *pOut++ = *r++;
            *pOut++ = *g++;
            *pOut = *b++;

          }

        }

      }

    }
    else if ( ( bpp == 2 ) && ( info.m_pixelRepresentation < 2 ) )
    {

      int32_t x, y;
      uint8_t* pIn = (uint8_t*)m_dcmImage.getInterData()->getData() +
                     i * sliceSize;

      if ( info.m_noFlip )
      {

        if ( m_proxy.isMemoryMapped() )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              *( (int16_t*)m_proxy( x, y, z, t ) ) = int16_t( *pIn++ );

            }

          }

        }
        else
        {

          uint32_t l = sliceSize;
          int16_t* p16 = (int16_t*)m_proxy( 0, 0, z, t );

          while ( l-- )
          {

            *p16++ = int16_t( *pIn++ );

          }

        }

      }
      else
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            info.m_patientOrientation.getDirect( x, y, z, px, py, pz );
            *( (int16_t*)m_proxy( px, py, pz, t ) ) = int16_t( *pIn++ );

          }

        }

      }

    }
    else
    {

      int32_t x, y;
      uint8_t* pIn = (uint8_t*)m_dcmImage.getInterData()->getData() +
                     i * sliceSize * bpp;

      if ( info.m_noFlip )
      {

        if ( m_proxy.isMemoryMapped() )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++, pIn += bpp )
            {

              std::memcpy( m_proxy( x, y, z, t ), pIn, bpp );

            }

          }

        }
        else
        {

          std::memcpy( m_proxy( 0, 0, z, t ), pIn, sliceSize * bpp );

        }

      }
      else
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++, pIn += bpp )
          {

            info.m_patientOrientation.getDirect( x, y, z, px, py, pz );
            std::memcpy( m_proxy( px, py, pz, t ), pIn, bpp );

          }

        }

      }

    }

    lock();
    m_count++;

    if ( m_progress )
    {

      m_progress->execute( 6 + 94 * m_count / n );

    }

    unlock();

  }

}
