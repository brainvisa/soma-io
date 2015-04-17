#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/PaletteImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/PaletteImage.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>


dcm::PaletteImage::PaletteImage( dcm::DicomProxy& proxy )
                 : dcm::ImagePixel( proxy )
{
}


bool dcm::PaletteImage::initialize( DcmDataset* dataset )
{

  if ( dataset )
  {

    return _paletteModule.parseItem( dataset );

  }

  return false;

}


void dcm::PaletteImage::fill( 
                            dcm::ImagePixel::Parameters& parameters, 
                            dcm::ImagePixel::OffsetParameters& offsetParameters,
                            void* imagePtr, 
                            int32_t z, 
                            int32_t t,
                            int32_t inputSlice )
{

  if ( imagePtr )
  {

    uint8_t* pIn = (uint8_t*)imagePtr + offsetParameters.offset +
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

          uint8_t* v = _proxy( x, y, slice, t );
          dcm::Rgba color = _paletteModule( int32_t( *pIn ) );

          *v++ = color._red;
          *v++ = color._green;
          *v = color._blue;

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

          uint8_t* v = _proxy( p.x - parameters.outLowerX, 
                               p.y - parameters.outLowerY, 
                               p.z - parameters.outLowerZ, 
                               t );
          dcm::Rgba color = _paletteModule( int32_t( *pIn ) );

          *v++ = color._red;
          *v++ = color._green;
          *v = color._blue;

        }

      }

    }

  }

}
