#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/RGBImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/RGBImage.h>
#include <Container/DicomProxy.h>
#endif


soma::RGBImage::RGBImage( soma::DicomProxy& proxy, int32_t planarConfiguration )
              : soma::ImagePixel( proxy ),
                _planarConfiguration( planarConfiguration )
{
}


void soma::RGBImage::fill( soma::ImagePixel::Parameters& parameters, 
                           soma::ImagePixel::OffsetParameters& offsetParameters,
                           void* imagePtr, 
                           int32_t z, 
                           int32_t t,
                           int32_t inputSlice )
{

  if ( imagePtr )
  {

    uint8_t* ptr = (uint8_t*)imagePtr;

    if ( _planarConfiguration )
    {

      uint8_t* r = ptr + offsetParameters.offset + 
                   3 * inputSlice * parameters.sizeXY;
      uint8_t* g = r + offsetParameters.rgbPlaneSize;
      uint8_t* b = g + offsetParameters.rgbPlaneSize;

      if ( parameters.noFlip )
      {

        int32_t startX = parameters.startX - parameters.outLowerX;
        int32_t endX = parameters.endX - parameters.outLowerX;
        int32_t startY = parameters.startY - parameters.outLowerY;
        int32_t endY = parameters.endY - parameters.outLowerY;
        int32_t slice = z - parameters.outLowerZ;
        int32_t x, y;

        for ( y = startY; y < endY; y++ )
        {

          for ( x = startX; x < endX; x++ )
          {

            uint8_t* p = _proxy( x, y, slice, t );

            *p++ = *r++;
            *p++ = *g++;
            *p = *b++;

          }

          r += offsetParameters.lineShift;
          g += offsetParameters.lineShift;
          b += offsetParameters.lineShift;

        }

      }
      else
      {

        int32_t x, y, px, py, pz;

        for ( y = parameters.startY; y < parameters.endY; y++ )
        {

          for ( x = parameters.startX; x < parameters.endX; x++ )
          {

            parameters.transform->getDirect( x, y, z, px, py, pz );

            uint8_t* p = _proxy( px - parameters.outLowerX, 
                                 py - parameters.outLowerY, 
                                 pz - parameters.outLowerZ, 
                                 t );

            *p++ = *r++;
            *p++ = *g++;
            *p = *b++;

          }

          r += offsetParameters.lineShift;
          g += offsetParameters.lineShift;
          b += offsetParameters.lineShift;

        }

      }

    }
    else
    {

      uint8_t* r = ptr + 3 * ( offsetParameters.offset + 
                               inputSlice * parameters.sizeXY );
      uint8_t* g = r + 1;
      uint8_t* b = g + 1;
      int32_t lineShift = 3 * offsetParameters.lineShift;

      if ( parameters.noFlip )
      {

        int32_t startX = parameters.startX - parameters.outLowerX;
        int32_t endX = parameters.endX - parameters.outLowerX;
        int32_t startY = parameters.startY - parameters.outLowerY;
        int32_t endY = parameters.endY - parameters.outLowerY;
        int32_t slice = z - parameters.outLowerZ;
        int32_t x, y;

        for ( y = startY; y < endY; y++ )
        {

          for ( x = startX; x < endX; x++, r += 3, g += 3, b += 3 )
          {

            uint8_t* p = _proxy( x, y, slice, t );

            *p++ = *r;
            *p++ = *g;
            *p = *b;

          }

          r += lineShift;
          g += lineShift;
          b += lineShift;

        }

      }
      else
      {

        int32_t x, y, px, py, pz;

        for ( y = parameters.startY; y < parameters.endY; y++ )
        {

          for ( x = parameters.startX; 
                x < parameters.endX; 
                x++, r += 3, g += 3, b += 3 )
          {

            parameters.transform->getDirect( x, y, z, px, py, pz );

            uint8_t* p = _proxy( px - parameters.outLowerX, 
                                 py - parameters.outLowerY, 
                                 pz - parameters.outLowerZ, 
                                 t );

            *p++ = *r;
            *p++ = *g;
            *p = *b;

          }

          r += lineShift;
          g += lineShift;
          b += lineShift;

        }

      }

    }

  }

}
