#include <Dicom/DicomReader.h>
#include <System/Directory.h>
#include <Container/Data.h>
#include <Pattern/Callback.h>
#include <Utils/StdInt.h>

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcxfer.h>


soma::DicomReader::DicomReader()
{
}


soma::DicomReader::~DicomReader()
{
}


std::string soma::DicomReader::getManufacturerName()
{

  return std::string( "Generic" );

}


bool soma::DicomReader::read( Directory& directory, 
                              soma::Data& data, 
                              soma::Callback* progress )
{

  initialize();

  if ( progress )
  {

    progress->execute( 0 );

  }

  std::string selectedFile = directory.getSelectedFile();

  if ( !selectedFile.empty() )
  {

    m_slices.push_back( selectedFile );

    if ( progress )
    {

      progress->execute( 3 );

    }

    // read global information from selected image
    if ( readHeader( selectedFile ) )
    {

      if ( progress )
      {

        progress->execute( 6 );

      }

      // read datset
      bool status = readData( data, progress );

      if ( progress )
      {

        progress->execute( 100 );

      }

      return status;

    }

  }

  return false;

}


bool soma::DicomReader::readHeader( const std::string& fileName )
{

  DcmFileFormat header;
  OFCondition status = header.loadFile( fileName.c_str() );

  if ( status.bad() )
  {

    return false;

  }

  DcmDataset* dataset = header.getDataset();

  if ( !dataset )
  {

    return false;

  }

  OFString tmpString;

  if ( dataset->findAndGetOFString( DCM_SeriesInstanceUID, tmpString ).good() )
  {

    m_seriesInstanceUID = tmpString.c_str();

  }
  else
  {

    return false;

  }
  
  Uint16 tmpShort;
  if ( dataset->findAndGetUint16( DCM_Columns, tmpShort ).good() )
  {

    m_dataInfo.m_width = (int32_t)tmpShort;

  }
  else
  {

    return false;

  }

  if ( dataset->findAndGetUint16( DCM_Rows, tmpShort ).good() )
  {

    m_dataInfo.m_height = (int32_t)tmpShort;

  }
  else
  {

    return false;

  }

  if ( dataset->findAndGetUint16( DCM_BitsAllocated, tmpShort ).good() )
  {

    m_dataInfo.m_depth = int32_t( tmpShort < 8 ? 8 : tmpShort );

  }
  else
  {

    return false;

  }

  if ( dataset->findAndGetUint16( DCM_BitsStored, tmpShort ).good() )
  {

    m_dataInfo.m_bitsStored = int32_t( tmpShort < 8 ? 8 : tmpShort );

  }
  else
  {

    return false;

  }

  if ( dataset->findAndGetOFString( DCM_PhotometricInterpretation, 
                                    tmpString ).good() )
  {

    m_dataInfo.m_monochrome = false;

    if ( !tmpString.compare( "MONOCHROME1" ) || 
         !tmpString.compare( "MONOCHROME2" ) )
    {

      m_dataInfo.m_monochrome = true;

    }

  }
  else
  {

    return false;

  }

  Float64 tmpFloat;
  if ( dataset->findAndGetFloat64( DCM_PixelSpacing, tmpFloat, 0 ).good() )
  {

    m_dataInfo.m_pixelSpacingX = (double)tmpFloat;

  }

  if ( dataset->findAndGetFloat64( DCM_PixelSpacing, tmpFloat, 1 ).good() )
  {

    m_dataInfo.m_pixelSpacingY = (double)tmpFloat;

  }

  if ( dataset->findAndGetFloat64( DCM_SpacingBetweenSlices,
                                   tmpFloat ).good() )
  {

    m_dataInfo.m_spacingBetweenSlices = (double)tmpFloat;

  }
  else
  {

    if ( dataset->findAndGetFloat64( DCM_SliceThickness, tmpFloat ).good() )
    {

      m_dataInfo.m_spacingBetweenSlices = (double)tmpFloat;

    }

  }

  m_dataInfo.m_pixelSpacingZ = m_dataInfo.m_spacingBetweenSlices;

  m_dataInfo.m_minimum = ( 1 << m_dataInfo.m_bitsStored ) - 1;
  m_dataInfo.m_maximum = 1 - ( 1 << m_dataInfo.m_bitsStored );

  return readHeader( dataset );

}


void soma::DicomReader::initialize()
{

  m_dataInfo.clear();
  m_seriesInstanceUID.clear();
  m_slices.clear();

}


bool soma::DicomReader::readHeader( DcmDataset* /* dataset */ )
{

  return true;

}
