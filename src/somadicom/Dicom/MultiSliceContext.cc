#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiSliceContext.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/MultiSliceContext.h>
#include <Container/DicomProxy.h>
#endif


soma::MultiSliceContext::MultiSliceContext( 
                                       DicomImage& dcmImage, 
                                       soma::DicomProxy& proxy,
                                       const std::vector< int32_t >& indexLut,
                                       const std::vector< int32_t >& selection )
                       : soma::LoopContext(),
                         _dcmImage( dcmImage ),
                         _proxy( proxy ),
                         _indexLut( indexLut ),
                         _selection( selection )
{
}


void soma::MultiSliceContext::doIt( int32_t startIndex, int32_t count )
{

  soma::DataInfo& info = _proxy.getDataInfo();
  int32_t i, stopIndex = startIndex + count;
  int32_t px, py, pz, sizeX, sizeY, sizeZ;
  int32_t bpp = info._bpp;
  int32_t startX = info._boundingBox.getLowerX();
  int32_t startY = info._boundingBox.getLowerY();
  int32_t endX = info._boundingBox.getUpperX() + 1;
  int32_t endY = info._boundingBox.getUpperY() + 1;
  soma::BoundingBox< int32_t > outBoundingBox;

  info._patientOrientation.getOnDiskSize( sizeX, sizeY, sizeZ );

  int32_t sliceSize = sizeX * sizeY;
  int32_t lineShift = sizeX + startX - endX;
  int32_t lineShift2 = lineShift * bpp;

  if ( info._noFlip )
  {

    outBoundingBox = info._boundingBox;

  }
  else
  {

    outBoundingBox =
             info._patientOrientation.getDirectBoundingBox( info._boundingBox );

  }

  int32_t bbLowerX = outBoundingBox.getLowerX();
  int32_t bbLowerY = outBoundingBox.getLowerY();
  int32_t bbLowerZ = outBoundingBox.getLowerZ();
  int32_t bbLowerT = outBoundingBox.getLowerT();

  for ( i = startIndex; i < stopIndex; i++ )
  {

    int32_t selectedIdx = _selection[ i ];
    int32_t index = _indexLut[ selectedIdx ];
    int32_t z = index % sizeZ;
    int32_t t = index / sizeZ;
    int32_t offset = selectedIdx * sliceSize + startY * sizeX + startX;

    if ( bpp == 3 )
    {

      int32_t x, y;
      uint8_t** d = (uint8_t**)_dcmImage.getInterData()->getData();
      uint8_t* r = d[ 0 ] + offset;
      uint8_t* g = d[ 1 ] + offset;
      uint8_t* b = d[ 2 ] + offset;

      if ( info._noFlip )
      {

        for ( y = startY; y < endY; y++ )
        {

          for ( x = startX; x < endX; x++ )
          {

            uint8_t* pOut = _proxy( x - bbLowerX, 
                                    y - bbLowerY, 
                                    z - bbLowerZ, 
                                    t - bbLowerT );

            *pOut++ = *r++;
            *pOut++ = *g++;
            *pOut = *b++;

          }

          r += lineShift;
          g += lineShift;
          b += lineShift;

        }

      }
      else
      {

        for ( y = startY; y < endY; y++ )
        {

          for ( x = startX; x < endX; x++ )
          {

            info._patientOrientation.getDirect( x, y, z, px, py, pz );

            uint8_t* pOut = _proxy( px - bbLowerX, 
                                    py - bbLowerY, 
                                    pz - bbLowerZ, 
                                    t - bbLowerT );

            *pOut++ = *r++;
            *pOut++ = *g++;
            *pOut = *b++;

          }

          r += lineShift;
          g += lineShift;
          b += lineShift;

        }

      }

    }
    else if ( ( bpp == 2 ) && ( info._pixelRepresentation < 2 ) )
    {

      int32_t x, y;
      uint8_t* pIn = (uint8_t*)_dcmImage.getInterData()->getData() + offset;

      if ( info._noFlip )
      {

        for ( y = startY; y < endY; y++, pIn += lineShift )
        {

          for ( x = startX; x < endX; x++ )
          {

            *( (int16_t*)_proxy( x - bbLowerX, 
                                 y - bbLowerY, 
                                 z - bbLowerZ, 
                                 t - bbLowerT ) ) = int16_t( *pIn++ );

          }

        }

      }
      else
      {

        for ( y = startY; y < endY; y++, pIn += lineShift )
        {

          for ( x = startX; x < endX; x++ )
          {

            info._patientOrientation.getDirect( x, y, z, px, py, pz );
            *( (int16_t*)_proxy( px - bbLowerX, 
                                 py - bbLowerY, 
                                 pz - bbLowerZ, 
                                 t - bbLowerT ) ) = int16_t( *pIn++ );

          }

        }

      }

    }
    else
    {

      int32_t x, y;
      uint8_t* pIn = (uint8_t*)_dcmImage.getInterData()->getData() +
                     offset * bpp;

      if ( info._noFlip )
      {

        if ( _proxy.isMemoryMapped() )
        {

          for ( y = startY; y < endY; y++, pIn += lineShift2 )
          {

            for ( x = startX; x < endX; x++, pIn += bpp )
            {

              std::memcpy( _proxy( x - bbLowerX, 
                                   y - bbLowerY, 
                                   z - bbLowerZ, 
                                   t - bbLowerT ), 
                           pIn, 
                           bpp );

            }

          }

        }
        else
        {

          int32_t length = bpp * ( endX - startX );
          int32_t lineShift3 = sizeX * bpp;

          for ( y = startY; y < endY; y++, pIn += lineShift3 )
          {

            std::memcpy( _proxy( 0, 
                                 y - bbLowerY, 
                                 z - bbLowerZ, 
                                 t - bbLowerT ), 
                         pIn, 
                         length );

          }

        }

      }
      else
      {

        for ( y = startY; y < endY; y++, pIn += lineShift2 )
        {

          for ( x = startX; x < endX; x++, pIn += bpp )
          {

            info._patientOrientation.getDirect( x, y, z, px, py, pz );
            std::memcpy( _proxy( px - bbLowerX, 
                                 py - bbLowerY, 
                                 pz - bbLowerZ, 
                                 t - bbLowerT ), 
                         pIn, 
                         bpp );

          }

        }

      }

    }

  }

}
