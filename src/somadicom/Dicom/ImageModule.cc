#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/ImageModule.h>
#else
#include <Dicom/ImageModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::ImageModule::ImageModule()
                 : soma::DicomModule(),
                   _samplesPerPixel( 0 ),
                   _photometricInterpretation( "" ),
                   _planarConfiguration( 0 ),
                   _rows( 0 ),
                   _columns( 0 ),
                   _pixelSpacingX( 1.0 ),
                   _pixelSpacingY( 1.0 ),
                   _bitsAllocated( 0 ),
                   _bitsStored( 0 ),
                   _pixelRepresentation( 0 ),
                   _smallestPixelValue( 0 ),
                   _largestPixelValue( 0 ),
                   _pixelPaddingValue( 0 )
{
}


bool soma::ImageModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    OFString tmpString;
    Uint16 tmpUint16;
    Sint16 tmpInt16;
    Float64 tmpDouble;

    if ( dcmItem->findAndGetUint16( DCM_SamplesPerPixel, tmpUint16 ).good() )
    {

      _samplesPerPixel = int32_t( tmpUint16 );

    }

    if ( dcmItem->findAndGetOFString( DCM_PhotometricInterpretation, 
                                      tmpString ).good() )
    {

      _photometricInterpretation = tmpString.c_str();

      if ( _photometricInterpretation == "PALETTE COLOR" )
      {

        _samplesPerPixel = 3;

      }

    }
    else
    {

      return false;

    }

    if ( dcmItem->findAndGetUint16( DCM_PlanarConfiguration, 
                                    tmpUint16 ).good() )
    {

      _planarConfiguration = int32_t( tmpUint16 );

    }

    if ( dcmItem->findAndGetUint16( DCM_Rows, tmpUint16 ).good() )
    {

      _rows = int32_t( tmpUint16 );

    }
    else
    {

      return false;

    }

    if ( dcmItem->findAndGetUint16( DCM_Columns, tmpUint16 ).good() )
    {

      _columns = int32_t( tmpUint16 );

    }
    else
    {

      return false;

    }

    if ( dcmItem->findAndGetUint16( DCM_BitsAllocated, tmpUint16 ).good() )
    {

      _bitsAllocated = int32_t( tmpUint16 );

    }
    else
    {

      return false;

    }

    if ( dcmItem->findAndGetUint16( DCM_BitsStored, tmpUint16 ).good() )
    {

      _bitsStored = int32_t( tmpUint16 );

    }
    else
    {

      return false;

    }

    if ( dcmItem->findAndGetUint16( DCM_PixelRepresentation, 
                                    tmpUint16 ).good() )
    {

      _pixelRepresentation = int32_t( tmpUint16 );

    }

    _smallestPixelValue = 0;

    if ( dcmItem->findAndGetUint16( DCM_SmallestImagePixelValue, 
                                    tmpUint16 ).good() )
    {

      _smallestPixelValue = int32_t( tmpUint16 );

    }
    else if ( dcmItem->findAndGetSint16( DCM_SmallestImagePixelValue, 
                                         tmpInt16 ).good() )
    {

      _smallestPixelValue = int32_t( tmpInt16 );

    }

    _largestPixelValue = 0;

    if ( dcmItem->findAndGetUint16( DCM_LargestImagePixelValue, 
                                    tmpUint16 ).good() )
    {

      _largestPixelValue = int32_t( tmpUint16 );

    }
    else if ( dcmItem->findAndGetSint16( DCM_LargestImagePixelValue, 
                                         tmpInt16 ).good() )
    {

      _largestPixelValue = int32_t( tmpInt16 );

    }

    if ( dcmItem->findAndGetFloat64( DCM_PixelSpacing, tmpDouble, 0 ).good() )
    {

      _pixelSpacingX = double( tmpDouble );

    }

    if ( dcmItem->findAndGetFloat64( DCM_PixelSpacing, tmpDouble, 1 ).good() )
    {

      _pixelSpacingY = double( tmpDouble );

    }

    if ( dcmItem->findAndGetSint16( DCM_PixelPaddingValue, tmpInt16 ).good() )
    {

      _pixelPaddingValue = int32_t( tmpInt16 );

    }

    return true;

  }

  return false;

}


int32_t soma::ImageModule::getSamplesPerPixel() const
{

  return _samplesPerPixel;

}


const std::string& soma::ImageModule::getPhotometricInterpretation() const
{

  return _photometricInterpretation;

}


int32_t soma::ImageModule::getPlanarConfiguration() const
{

  return _planarConfiguration;

}


int32_t soma::ImageModule::getRows() const
{

  return _rows;

}


int32_t soma::ImageModule::getColumns() const
{

  return _columns;

}


double soma::ImageModule::getPixelSpacingX() const
{

  return _pixelSpacingX;

}


double soma::ImageModule::getPixelSpacingY() const
{

  return _pixelSpacingY;

}


int32_t soma::ImageModule::getBitsAllocated() const
{

  return _bitsAllocated;

}


int32_t soma::ImageModule::getBitsStored() const
{

  return _bitsStored;

}


int32_t soma::ImageModule::getPixelRepresentation() const
{

  return _pixelRepresentation;

}


int32_t soma::ImageModule::getSmallestPixelValue() const
{

  return _smallestPixelValue;

}


int32_t soma::ImageModule::getLargestPixelValue() const
{

  return _largestPixelValue;

}


int32_t soma::ImageModule::getPixelPaddingValue() const
{

  return _pixelPaddingValue;

}
