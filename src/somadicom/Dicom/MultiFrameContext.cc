#include <soma-io/Dicom/MultiFrameContext.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Pattern/Callback.h>


soma::MultiFrameContext::MultiFrameContext( DicomImage& dcmImage, 
                                            soma::DicomProxy& proxy,
                                            bool fillSlices,
                                            soma::Callback* progress )
                       : carto::LoopContext(),
                         m_dcmImage( dcmImage ),
                         m_proxy( proxy ),
                         m_fillSlices( fillSlices ),
                         m_progress( progress ),
                         m_count( 0 )
{
}


void soma::MultiFrameContext::doIt( int32_t startIndex, int32_t count )
{

  soma::DataInfo& info = m_proxy.getDataInfo();
  int32_t bpp = info.m_bpp;
  uint32_t sliceSize = info.m_sliceSize;
  int32_t sizeX = info.m_width;
  int32_t sizeY = info.m_height;
  int32_t stopIndex = startIndex + count;
  int32_t i, n;

  if ( m_fillSlices )
  {

    n = info.m_slices - 1;

  }
  else
  {

    n = info.m_frames - 1;

  }

  if ( bpp == 3 )
  {

    int32_t offset = startIndex * sliceSize;
    uint8_t** d = (uint8_t**)m_dcmImage.getInterData()->getData();
    uint8_t* r = d[ 0 ] + offset;
    uint8_t* g = d[ 1 ] + offset;
    uint8_t* b = d[ 2 ] + offset;
    uint8_t* p;

    if ( m_fillSlices )
    {

      if ( m_proxy.isMemoryMapped() )
      {

        for ( i = startIndex; i < stopIndex; i++  )
        {

          int32_t x, y;

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              p = m_proxy( x, y, i );
              *p++ = *r++;
              *p++ = *g++;
              *p++ = *b++;

            }

          }

        }

      }
      else
      {

        i = count * sliceSize;
        p = m_proxy( 0, 0, startIndex );

        while ( i-- )
        {

          *p++ = *r++;
          *p++ = *g++;
          *p++ = *b++;

        }

      }

    }
    else
    {

      if ( m_proxy.isMemoryMapped() )
      {

        for ( i = startIndex; i < stopIndex; i++  )
        {

          int32_t x, y;

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              p = m_proxy( x, y, 0, i );
              *p++ = *r++;
              *p++ = *g++;
              *p++ = *b++;

            }

          }

        }

      }
      else
      {

        for ( i = startIndex; i < stopIndex; i++  )
        {

          int32_t l = sliceSize;

          p = m_proxy( 0, 0, 0, i );

          while ( l-- )
          {

            *p++ = *r++;
            *p++ = *g++;
            *p++ = *b++;

          }

        }

      }

    }

  }
  else if ( ( bpp == 2 ) && ( info.m_pixelRepresentation < 2 ) )
  {

    uint8_t* d = (uint8_t*)m_dcmImage.getInterData()->getData() +
                 startIndex * sliceSize;

    if ( m_fillSlices )
    {

      if ( m_proxy.isMemoryMapped() )
      {

        for ( i = startIndex; i < stopIndex; i++  )
        {

          int32_t x, y;

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              *( (int16_t*)m_proxy( x, y, i ) ) = int16_t( *d++ );

            }

          }

        }

      }
      else
      {

        int16_t* p16 = (int16_t*)m_proxy( 0, 0, startIndex );

        i = count * sliceSize;

        while ( i-- )
        {

          *p16++ = int16_t( *d++ );

        }

       }

    }
    else
    {

      if ( m_proxy.isMemoryMapped() )
      {

        for ( i = startIndex; i < stopIndex; i++  )
        {

          int32_t x, y;

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              *( (int16_t*)m_proxy( x, y, 0, i ) ) = int16_t( *d++ );

            }

          }

        }

      }
      else
      {

        for ( i = startIndex; i < stopIndex; i++  )
        {

          int32_t l = sliceSize;
          int16_t* p16 = (int16_t*)m_proxy( 0, 0, 0, i );

          while ( l-- )
          {

            *p16++ = int16_t( *d++ );

          }

        }

      }

    }

  }
  else
  {

    sliceSize *= bpp;

    uint8_t* d = (uint8_t*)m_dcmImage.getInterData()->getData() +
                 startIndex * sliceSize;

    if ( m_fillSlices )
    {

      if ( m_proxy.isMemoryMapped() )
      {

        for ( i = startIndex; i < stopIndex; i++  )
        {

          int32_t x, y;

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++, d += bpp )
            {

              std::memcpy( m_proxy( x, y, i ), d, bpp );

            }

          }

        }

      }
      else
      {

        std::memcpy( m_proxy( 0, 0, startIndex ), d, count * sliceSize );

      }

    }
    else
    {

      if ( m_proxy.isMemoryMapped() )
      {

        for ( i = startIndex; i < stopIndex; i++  )
        {

          int32_t x, y;

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++, d += bpp )
            {

              std::memcpy( m_proxy( x, y, 0, i ), d, bpp );

            }

          }

        }

      }
      else
      {

        for ( i = startIndex; i < stopIndex; i++, d += sliceSize  )
        {

          std::memcpy( m_proxy( 0, 0, 0, i ), d, sliceSize );

        }

      }

    }

  }

  lock();
  m_count += count;

  if ( m_progress )
  {

    m_progress->execute( 6 + 94 * m_count / n );

  }

  unlock();

}
