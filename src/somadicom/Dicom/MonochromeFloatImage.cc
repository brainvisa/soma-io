#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MonochromeFloatImage.h>
#include <soma-io/Dicom/ModalityLutModule.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/MonochromeFloatImage.h>
#include <Dicom/ModalityLutModule.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>


template < typename T >
dcm::MonochromeFloatImage< T >::MonochromeFloatImage( dcm::DicomProxy& proxy )
                              : dcm::ImagePixel( proxy )
{
}


template < typename T >
inline
bool dcm::MonochromeFloatImage< T >::initialize( DcmDataset* dataset )
{

  if ( dataset )
  {

    dcm::ModalityLutModule modalityLutModule;

    if ( modalityLutModule.parseItem( dataset ) )
    {

      _intercept = (float)modalityLutModule.getRescaleIntercept()[ 0 ];
      _slope = (float)modalityLutModule.getRescaleSlope()[ 0 ];

    }

    return true;

  }

  return false;

}


template < typename T >
inline
void dcm::MonochromeFloatImage< T >::fill(
                            dcm::ImagePixel::Parameters& parameters,
                            dcm::ImagePixel::OffsetParameters& offsetParameters,
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
      int32_t x, y;

      for ( y = startY; y < endY; y++, pIn += offsetParameters.lineShift )
      {

        for ( x = startX; x < endX; x++, pIn++ )
        {

          *( (float*)_proxy( x, y, slice, t ) ) = float( *pIn ) * _slope +
                                                  _intercept;

        }

      }

    }
    else
    {

      int32_t x, y;
      dcm::Vector3d< int32_t > p;

      for ( y = parameters.startY; 
            y < parameters.endY; 
            y++, pIn += offsetParameters.lineShift )
      {

        for ( x = parameters.startX; x < parameters.endX; x++, pIn++ )
        {

          p = parameters.transform->getDirect( x, y, z );
          *( (float*)_proxy( p.x - parameters.outLowerX, 
                             p.y - parameters.outLowerY, 
                             p.z - parameters.outLowerZ, 
                             t ) ) = float( *pIn ) * _slope + _intercept;

        }

      }

    }

  }

}


template class dcm::MonochromeFloatImage< uint8_t >;
template class dcm::MonochromeFloatImage< int8_t >;
template class dcm::MonochromeFloatImage< uint16_t >;
template class dcm::MonochromeFloatImage< int16_t >;
