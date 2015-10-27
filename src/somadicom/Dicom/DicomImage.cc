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


dcm::DicomImage::DicomImage( dcm::DicomProxy& proxy,
                             dcm::ImagePixel::Parameters& parameters )
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


dcm::DicomImage::~DicomImage()
{

  delete _image;

}


bool dcm::DicomImage::load( const std::string& fileName )
{

  if ( !fileName.empty() )
  {

    _dicomFile.clear();

    if ( _dicomFile.loadFile( fileName.c_str() ).good() )
    {

      return getPixelData( _dicomFile.getDataset() );

    }

  }

  return false;

}


void dcm::DicomImage::getImagePtr()
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


void dcm::DicomImage::fillIndex( int32_t index, int32_t inputSlice )
{

  fill( index % _parameters.sizeZ, index / _parameters.sizeZ, inputSlice );

}


void dcm::DicomImage::fill( int32_t z, int32_t t, int32_t inputSlice )
{

  if ( _imagePtr && _image )
  {

    _image->fill( _parameters, 
                  _offsetParameters,
                  _imagePtr, 
                  z, 
                  t - _parameters.outLowerT, 
                  inputSlice );

  }

}


void dcm::DicomImage::getMinMaxValues( int32_t& min, int32_t& max )
{

  min = _imageModule.getSmallestPixelValue();
  max = _imageModule.getLargestPixelValue();

}


bool dcm::DicomImage::getPixelData( DcmDataset* dataset )
{

  if ( dataset )
  {

    DcmElement* element = 0;
    
    _dataset = dataset;

    if ( dataset->findAndGetElement( DCM_PixelData, element ).good() )
    {

      _pixelData = OFstatic_cast( DcmPixelData*, element );

      if ( _pixelData && _imageModule.parseItem( dataset ) )
      {

        chooseImagePixel( _imageModule.getPhotometricInterpretation() );

        if ( _image )
        {

          getImagePtr();
          _image->initialize( dataset );

          return true;

        }

      }

    }

  }

  return false;

}


void dcm::DicomImage::chooseImagePixel( const std::string& photometric )
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

      if ( ( _dcmEVR == EVR_OW ) && ( _proxy.getDataInfo()._depth > 8 ) )
      {

        if ( _proxy.getDataInfo()._pixelRepresentation )
        {

          _image = new dcm::MonochromeImage< int16_t >( _proxy );

        }
        else
        {

          _image = new dcm::MonochromeImage< uint16_t >( _proxy );

        }

      }
      else
      {

        if ( _proxy.getDataInfo()._pixelRepresentation )
        {

          _image = new dcm::MonochromeImage< int8_t >( _proxy );

        }
        else
        {

          _image = new dcm::MonochromeImage< uint8_t >( _proxy );

        }

      }

      if ( _photometric == "MONOCHROME1" )
      {

        _proxy.getDataInfo()._invertLut = true;

      }

    }
    else if ( _photometric == "PALETTE COLOR" )
    {

      _image = new dcm::PaletteImage( _proxy );

    }
    else /* if ( ( _photometric == "RGB" ) ||
                 ( _photometric == "YBR_FULL_422" ) ) */
    {

      _image = new dcm::RGBImage( _proxy, 
                                  _imageModule.getPlanarConfiguration() );

    }

  }

}
