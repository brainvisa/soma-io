#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/RGBImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/RGBImage.h>
#include <Container/DicomProxy.h>
#endif


dcm::RGBImage::RGBImage( dcm::DicomProxy& proxy, int32_t planarConfiguration )
             : dcm::ImagePixel( proxy ),
               _planarConfiguration( planarConfiguration )
{
}


void dcm::RGBImage::fill( dcm::ImagePixel::Parameters& parameters, 
                          dcm::ImagePixel::OffsetParameters& offsetParameters,
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

            uint8_t* v = _proxy( x, y, slice, t );

            *v++ = *r++;
            *v++ = *g++;
            *v = *b++;

          }

          r += offsetParameters.lineShift;
          g += offsetParameters.lineShift;
          b += offsetParameters.lineShift;

        }

      }
      else
      {

        int32_t x, y;
        dcm::Vector3d< int32_t > p;

        for ( y = parameters.startY; y < parameters.endY; y++ )
        {

          for ( x = parameters.startX; x < parameters.endX; x++ )
          {

            p = parameters.transform->getDirect( x, y, z );

            uint8_t* v = _proxy( p.x - parameters.outLowerX, 
                                 p.y - parameters.outLowerY, 
                                 p.z - parameters.outLowerZ, 
                                 t );

            *v++ = *r++;
            *v++ = *g++;
            *v = *b++;

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

            uint8_t* v = _proxy( x, y, slice, t );

            *v++ = *r;
            *v++ = *g;
            *v = *b;

          }

          r += lineShift;
          g += lineShift;
          b += lineShift;

        }

      }
      else
      {

        int32_t x, y;
        dcm::Vector3d< int32_t > p;

        for ( y = parameters.startY; y < parameters.endY; y++ )
        {

          for ( x = parameters.startX; 
                x < parameters.endX; 
                x++, r += 3, g += 3, b += 3 )
          {

            p = parameters.transform->getDirect( x, y, z );

            uint8_t* v = _proxy( p.x - parameters.outLowerX, 
                                 p.y - parameters.outLowerY, 
                                 p.z - parameters.outLowerZ, 
                                 t );

            *v++ = *r;
            *v++ = *g;
            *v = *b;

          }

          r += lineShift;
          g += lineShift;
          b += lineShift;

        }

      }

    }

  }

}
