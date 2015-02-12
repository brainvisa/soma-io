#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/ImageModule.h>
#else
#include <Dicom/ImageModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::ImageModule::ImageModule()
                 : soma::DicomModule(),
                   _photometricInterpretation( "" ),
                   _rows( 0 ),
                   _columns( 0 ),
                   _pixelSpacingX( 1.0 ),
                   _pixelSpacingY( 1.0 ),
                   _bitsAllocated( 0 ),
                   _bitsStored( 0 ),
                   _pixelPaddingValue( 0 )
{
}


bool soma::ImageModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    OFString tmpString;
    Uint16 tmpUint16;
    Sint16 tmpInt16;
    Float64 tmpDouble;

    if ( dataset->findAndGetOFString( DCM_PhotometricInterpretation, 
                                      tmpString ).good() )
    {

      _photometricInterpretation = tmpString.c_str();

    }
    else
    {

      return false;

    }

    if ( dataset->findAndGetUint16( DCM_Rows, tmpUint16 ).good() )
    {

      _rows = int32_t( tmpUint16 );

    }
    else
    {

      return false;

    }

    if ( dataset->findAndGetUint16( DCM_Columns, tmpUint16 ).good() )
    {

      _columns = int32_t( tmpUint16 );

    }
    else
    {

      return false;

    }

    if ( dataset->findAndGetUint16( DCM_BitsAllocated, tmpUint16 ).good() )
    {

      _bitsAllocated = int32_t( tmpUint16 );

    }
    else
    {

      return false;

    }

    if ( dataset->findAndGetUint16( DCM_BitsStored, tmpUint16 ).good() )
    {

      _bitsStored = int32_t( tmpUint16 );

    }
    else
    {

      return false;

    }

    if ( dataset->findAndGetFloat64( DCM_PixelSpacing, tmpDouble, 0 ).good() )
    {

      _pixelSpacingX = double( tmpDouble );

    }

    if ( dataset->findAndGetFloat64( DCM_PixelSpacing, tmpDouble, 1 ).good() )
    {

      _pixelSpacingY = double( tmpDouble );

    }

    if ( dataset->findAndGetSint16( DCM_PixelPaddingValue, tmpInt16 ).good() )
    {

      _pixelPaddingValue = int32_t( tmpInt16 );

    }

    return true;

  }

  return false;

}


const std::string& soma::ImageModule::getPhotometricInterpretation() const
{

  return _photometricInterpretation;

}


bool soma::ImageModule::isMonochrome() const
{

  if ( ( _photometricInterpretation == "MONOCHROME1" ) ||
       ( _photometricInterpretation == "MONOCHROME2" ) )
  {

    return true;

  }

  return false;

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


int32_t soma::ImageModule::getPixelPaddingValue() const
{

  return _pixelPaddingValue;

}

