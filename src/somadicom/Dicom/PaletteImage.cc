#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/PaletteImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/PaletteImage.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>


soma::PaletteImage::PaletteImage( soma::DicomProxy& proxy )
                  : soma::ImagePixel( proxy )
{
}


bool soma::PaletteImage::initialize( DcmDataset* dataset )
{

  if ( dataset )
  {

    return _paletteModule.parseDataset( dataset );

  }

  return false;

}


void soma::PaletteImage::fill( 
                           soma::ImagePixel::Parameters& parameters, 
                           soma::ImagePixel::OffsetParameters& offsetParameters,
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

          uint8_t* p = _proxy( x, y, slice, t );
          soma::Rgba color = _paletteModule( int32_t( *pIn ) );

          *p++ = color._red;
          *p++ = color._green;
          *p = color._blue;

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

          uint8_t*p = _proxy( px - parameters.outLowerX, 
                              py - parameters.outLowerY, 
                              pz - parameters.outLowerZ, 
                              t );
          soma::Rgba color = _paletteModule( int32_t( *pIn ) );

          *p++ = color._red;
          *p++ = color._green;
          *p = color._blue;

        }

      }

    }

  }

}
