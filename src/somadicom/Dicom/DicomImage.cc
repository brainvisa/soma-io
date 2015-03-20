#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomImage.h>
#include <soma-io/Dicom/MonochromeImage.h>
#include <soma-io/Dicom/RGBImage.h>
#include <soma-io/Dicom/PaletteImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/DicomImage.h>
#include <Dicom/MonochromeImage.h>
#include <Dicom/RGBImage.h>
#include <Dicom/PaletteImage.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcxfer.h>
#include <dcmtk/dcmdata/dcstack.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::DicomImage::DicomImage( soma::DicomProxy& proxy,
                              soma::ImagePixel::Parameters& parameters )
                : _proxy( proxy ),
                  _parameters( parameters ),
                  _dataset( 0 ),
                  _pixelData( 0 ),
                  _image( 0 ),
                  _photometric( "" ),
                  _dcmEVR( EVR_OB ),
                  _imagePtr( 0 )
{

  _offsetParameters.offset = _parameters.startY * _parameters.sizeX + 
                             _parameters.startX;
  _offsetParameters.lineShift = _parameters.sizeX + _parameters.startX - 
                                _parameters.endX;
  _offsetParameters.rgbPlaneSize = _parameters.sizeXY;

}


soma::DicomImage::~DicomImage()
{

  delete _image;

}


bool soma::DicomImage::load( const std::string& fileName )
{

  if ( !fileName.empty() )
  {

    _dicomFile.clear();

    if ( _dicomFile.loadFile( fileName.c_str() ).good() )
    {

      _dataset = _dicomFile.getDataset();

      if ( _dataset )
      {

        DcmElement* element = 0;

        if ( _dataset->findAndGetElement( DCM_PixelData, element ).good() )
        {

          _pixelData = OFstatic_cast( DcmPixelData*, element );

          if ( _pixelData && _imageModule.parseItem( _dataset ) )
          {

            chooseImagePixel( _imageModule.getPhotometricInterpretation() );

            if ( _image )
            {

              getImagePtr();
              _image->initialize( _dataset );

              return true;

            }

          }

        }

      }

    }

  }

  return false;

}


void soma::DicomImage::getImagePtr()
{

  DcmXfer xfer( _dataset->getOriginalXfer() );

  _imagePtr = 0;

  if ( xfer.isEncapsulated() )
  {

    DcmStack stack;

    stack.push( _dataset );
    stack.push( _pixelData );

    if ( _pixelData->chooseRepresentation( EXS_LittleEndianExplicit, 
                                           0, stack ).good() )
    {

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
      OFString photo;

      _pixelData->getDecompressedColorModel( _dataset, photo );
      chooseImagePixel( photo.c_str() );
#else
      chooseImagePixel( _photometric );
#endif

    }

  }

  if ( _dcmEVR == EVR_OW )
  {

    Uint16* ptr = 0;

    _pixelData->getUint16Array( (Uint16*&)ptr );
    _imagePtr = (void*)ptr;

  }
  else
  {

    Uint8* ptr = 0;

    _pixelData->getUint8Array( (Uint8*&)ptr );
    _imagePtr = (void*)ptr;

  }

}


void soma::DicomImage::fillIndex( int32_t index, int32_t inputSlice )
{

  fill( index % _parameters.sizeZ, index / _parameters.sizeZ, inputSlice );

}


void soma::DicomImage::fill( int32_t z, int32_t t, int32_t inputSlice )
{

  if ( _imagePtr )
  {

    _image->fill( _parameters, 
                  _offsetParameters,
                  _imagePtr, 
                  z, 
                  t - _parameters.outLowerT, 
                  inputSlice );

  }

}


void soma::DicomImage::getMinMaxValues( int32_t& min, int32_t& max )
{

  min = _imageModule.getSmallestPixelValue();
  max = _imageModule.getLargestPixelValue();

}


void soma::DicomImage::chooseImagePixel( const std::string& photometric )
{

  if ( !photometric.empty() && 
       ( ( _photometric != photometric ) || 
         ( _dcmEVR != _pixelData->getVR() ) ) )
  {

    delete _image;

    _image = 0;
    _photometric = photometric;
    _dcmEVR = _pixelData->getVR();

    if ( ( _photometric == "MONOCHROME1" ) || 
         ( _photometric == "MONOCHROME2" ) )
    {

      if ( _dcmEVR == EVR_OW )
      {

        _image = new soma::MonochromeImage< uint16_t >( _proxy );

      }
      else
      {

        _image = new soma::MonochromeImage< uint8_t >( _proxy );

      }

      if ( _photometric == "MONOCHROME1" )
      {

        _proxy.getDataInfo()._invertLut = true;

      }

    }
    else if ( _photometric == "RGB" )
    {

      _image = new soma::RGBImage( 
                            _proxy, 
                            _imageModule.getPlanarConfiguration() );

    }
    else if ( _photometric == "PALETTE COLOR" )
    {

      _image = new soma::PaletteImage( _proxy );

    }

  }

}
