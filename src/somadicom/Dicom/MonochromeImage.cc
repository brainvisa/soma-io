#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MonochromeImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/MonochromeImage.h>
#include <Container/DicomProxy.h>
#endif

#include <cstring>


template < typename T >
soma::MonochromeImage< T >::MonochromeImage( soma::DicomProxy& proxy )
                          : soma::ImagePixel( proxy )
{
}


template < typename T >
inline
void soma::MonochromeImage< T >::fill(
                           soma::ImagePixel::Parameters& parameters,
                           soma::ImagePixel::OffsetParameters& offsetParameters, 
                           void* imagePtr, 
                           int32_t z, 
                           int32_t t,
                           int32_t inputSlice )
{

  if ( imagePtr )
  {

    T* pIn = (T*)imagePtr + offsetParameters.offset + 
             inputSlice * parameters.sizeXY;

    if ( parameters.noFlip )
    {

      int32_t startX = parameters.startX - parameters.outLowerX;
      int32_t endX = parameters.endX - parameters.outLowerX;
      int32_t startY = parameters.startY - parameters.outLowerY;
      int32_t endY = parameters.endY - parameters.outLowerY;
      int32_t slice = z - parameters.outLowerZ;

      if ( _proxy.isMemoryMapped() )
      {

        int32_t x, y;

        for ( y = startY; y < endY; y++, pIn += offsetParameters.lineShift )
        {

          for ( x = startX; x < endX; x++, pIn++ )
          {

            *( (T*)_proxy( x, y, slice, t ) ) = *pIn;

          }

        }

      }
      else
      {

        int32_t length = ( endX - startX ) * sizeof( T );
        int32_t y;

        for ( y = startY; y < endY; y++, pIn += parameters.sizeX )
        {

          std::memcpy( _proxy( startX, y, slice, t ), pIn, length );

        }

      }

    }
    else
    {

      int32_t x, y, px, py, pz;

      for ( y = parameters.startY; 
            y < parameters.endY; 
            y++, pIn += offsetParameters.lineShift )
      {

        for ( x = parameters.startX; x < parameters.endX; x++, pIn++ )
        {

          parameters.transform->getDirect( x, y, z, px, py, pz );
          *( (T*)_proxy( px - parameters.outLowerX, 
                         py - parameters.outLowerY, 
                         pz - parameters.outLowerZ, 
                         t ) ) = *pIn;

        }

      }

    }

  }

}


template class soma::MonochromeImage< uint8_t >;
template class soma::MonochromeImage< uint16_t >;
