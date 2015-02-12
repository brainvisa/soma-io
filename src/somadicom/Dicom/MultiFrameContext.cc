#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiFrameContext.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/MultiFrameContext.h>
#include <Container/DicomProxy.h>
#endif


soma::MultiFrameContext::MultiFrameContext( DicomImage& dcmImage, 
                                            soma::DicomProxy& proxy,
                                            bool fillSlices )
                       : soma::LoopContext(),
                         _dcmImage( dcmImage ),
                         _proxy( proxy ),
                         _fillSlices( fillSlices )
{
}


void soma::MultiFrameContext::doIt( int32_t startIndex, int32_t count )
{

  soma::DataInfo& info = _proxy.getDataInfo();
  int32_t bpp = info._bpp;
  uint32_t sliceSize = info._sliceSize;
  int32_t sizeX = info._width;
  int32_t stopIndex = startIndex + count;
  int32_t startX = info._boundingBox.getLowerX();
  int32_t startY = info._boundingBox.getLowerY();
  int32_t startZ = info._boundingBox.getLowerZ();
  int32_t startT = info._boundingBox.getLowerT();
  int32_t endX = info._boundingBox.getUpperX() + 1;
  int32_t endY = info._boundingBox.getUpperY() + 1;
  int32_t offset = startIndex * sliceSize + startY * sizeX + startX;
  int32_t lineShift = sizeX + startX - endX;
  int32_t i;

  if ( bpp == 3 )
  {

    uint8_t** d = (uint8_t**)_dcmImage.getInterData()->getData();
    uint8_t* r = d[ 0 ] + offset;
    uint8_t* g = d[ 1 ] + offset;
    uint8_t* b = d[ 2 ] + offset;
    uint8_t* p;

    if ( _fillSlices )
    {

      for ( i = startIndex; i < stopIndex; i++ )
      {

        int32_t x, y;

        for ( y = startY; y < endY; y++ )
        {

          for ( x = startX; x < endX; x++ )
          {

            p = _proxy( x - startX, y - startY, i - startZ );
            *p++ = *r++;
            *p++ = *g++;
            *p++ = *b++;

          }

          r += lineShift;
          g += lineShift;
          b += lineShift;

        }

      }

    }
    else
    {

      for ( i = startIndex; i < stopIndex; i++ )
      {

        int32_t x, y;

        for ( y = startY; y < endY; y++ )
        {

          for ( x = startX; x < endX; x++ )
          {

            p = _proxy( x - startX, y - startY, 0, i - startT );
            *p++ = *r++;
            *p++ = *g++;
            *p++ = *b++;

          }

          r += lineShift;
          g += lineShift;
          b += lineShift;

        }

      }

    }

  }
  else if ( ( bpp == 2 ) && ( info._pixelRepresentation < 2 ) )
  {

    uint8_t* d = (uint8_t*)_dcmImage.getInterData()->getData() + offset;

    if ( _fillSlices )
    {

      for ( i = startIndex; i < stopIndex; i++ )
      {

        int32_t x, y;

        for ( y = startY; y < endY; y++, d += lineShift )
        {

          for ( x = startX; x < endX; x++ )
          {

            *( (int16_t*)_proxy( x - startX, y - startY, i - startZ ) ) = 
                                                                int16_t( *d++ );

          }

        }

      }

    }
    else
    {

      for ( i = startIndex; i < stopIndex; i++ )
      {

        int32_t x, y;

        for ( y = startY; y < endY; y++, d += lineShift )
        {

          for ( x = startX; x < endX; x++ )
          {

            *( (int16_t*)_proxy( x - startX, y - startY, 0, i - startT ) ) = 
                                                                int16_t( *d++ );

          }

        }

      }

    }

  }
  else
  {

    uint8_t* d = (uint8_t*)_dcmImage.getInterData()->getData() + offset * bpp;

    lineShift *= bpp;

    if ( _fillSlices )
    {

      if ( _proxy.isMemoryMapped() )
      {

        for ( i = startIndex; i < stopIndex; i++ )
        {

          int32_t x, y;

          for ( y = startY; y < endY; y++, d += lineShift )
          {

            for ( x = startX; x < endX; x++, d += bpp )
            {

              std::memcpy( _proxy( x - startX, y - startY, i - startZ ), 
                           d, 
                           bpp );

            }

          }

        }

      }
      else
      {

        int32_t length = bpp * ( endX - startX );
        int32_t lineShift2 = sizeX * bpp;
        int32_t y;

        for ( i = startIndex; i < stopIndex; i++ )
        {

          for ( y = startY; y < endY; y++, d += lineShift2 )
          {

            std::memcpy( _proxy( 0, y - startY, i - startZ ), d, length );

          }

        }

      }

    }
    else
    {

      if ( _proxy.isMemoryMapped() )
      {

        for ( i = startIndex; i < stopIndex; i++ )
        {

          int32_t x, y;

          for ( y = startY; y < endY; y++, d += lineShift )
          {

            for ( x = startX; x < endX; x++, d += bpp )
            {

              std::memcpy( _proxy( x - startX, y - startY, 0, i - startT ), 
                           d, 
                           bpp );

            }

          }

        }

      }
      else
      {

        int32_t length = bpp * ( endX - startX );
        int32_t lineShift2 = sizeX * bpp;
        int32_t y;

        for ( i = startIndex; i < stopIndex; i++ )
        {

          for ( y = startY; y < endY; y++, d += lineShift2 )
          {

            std::memcpy( _proxy( 0, y - startY, 0, i - startT ), d, length );

          }

        }

      }

    }

  }

}
