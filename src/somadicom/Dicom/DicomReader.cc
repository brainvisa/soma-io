#include <soma-io/Dicom/DicomReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/System/DirectoryParser.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcxfer.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::DicomReader::DicomReader()
                 : m_dataInfo( 0 )
{
}


soma::DicomReader::~DicomReader()
{
}


std::string soma::DicomReader::getManufacturerName()
{

  return std::string( "Generic" );

}


bool soma::DicomReader::check( soma::DirectoryParser& directory,
                               std::vector< std::string >& fileList,
                               soma::DataInfo& dataInfo,
                               soma::DicomDatasetHeader& datasetHeader )
{

  std::string selectedFile = directory.getSelectedFile();

  if ( !selectedFile.empty() )
  {

    m_dataInfo = &dataInfo;

    m_seriesInstanceUID.clear();
    m_slices.clear();

    if ( readHeader( selectedFile ) )
    {

      fileList = sortFiles( directory );

      if ( fileList.size() )
      {

        int32_t i = 0;
        std::vector< std::string >::iterator
          f = fileList.begin(),
          fe = fileList.end();

        datasetHeader.allocate( m_dataInfo->m_fileCount );

        while ( f != fe )
        {

          DcmFileFormat fileFormat;

          if ( fileFormat.loadFile( f->c_str() ).good() )
          {

            datasetHeader.add( fileFormat.getDataset(), i );

          }
          
          ++f;
          i++;

        }

        m_dataInfo->initialize();

        return true;

      }

    }

  }

  return false;

}


bool soma::DicomReader::getHeader( soma::HeaderProxy& header, 
                                   soma::DataInfo& info,
                                   soma::DicomDatasetHeader& datasetHeader )
{

  header.addAttribute( "sizeX", info.m_width );
  header.addAttribute( "sizeY", info.m_height );
  header.addAttribute( "sizeZ", info.m_slices );
  header.addAttribute( "sizeT", info.m_frames );

  header.addAttribute( "resolutionX", info.m_resolution.x );
  header.addAttribute( "resolutionY", info.m_resolution.y );
  header.addAttribute( "resolutionZ", info.m_resolution.z );
  header.addAttribute( "resolutionT", info.m_repetitionTime );

  header.addAttribute( "series_instance_uid", m_seriesInstanceUID );

  if ( info.m_slope.size() > 0 )
  {

    header.addAttribute( "rescale_slope", info.m_slope );

  }

  if ( info.m_intercept.size() > 0 )
  {

    header.addAttribute( "rescale_intercept", info.m_intercept );

  }

  OFString tmpString;
  Sint32 tmpInt;
  DcmDataset dataset;

  datasetHeader.get( dataset );

  if ( dataset.findAndGetOFString( DCM_AcquisitionDate, tmpString ).good() )
  {

    header.addAttribute( "acquisition_date", tmpString.c_str() );

  }

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
  if ( dataset.findAndGetOFString( DCM_PatientName, tmpString ).good() )
#else
  if ( dataset.findAndGetOFString( DCM_PatientsName, tmpString ).good() )
#endif
  {

    header.addAttribute( "patient_name", tmpString.c_str() );

  }
  
  if ( dataset.findAndGetOFString( DCM_StudyDate, tmpString ).good() )
  {

    header.addAttribute( "study_date", tmpString.c_str() );

  }

  if ( dataset.findAndGetOFString( DCM_StudyTime, tmpString ).good() )
  {

    header.addAttribute( "study_time", tmpString.c_str() );

  }

  if ( dataset.findAndGetOFString( DCM_PatientID, tmpString ).good() )
  {

    header.addAttribute( "patient_id", tmpString.c_str() );

  }

  if ( dataset.findAndGetOFString( DCM_StudyID, tmpString ).good() )
  {

    header.addAttribute( "study_id", tmpString.c_str() );

  }

  if ( dataset.findAndGetSint32( DCM_SeriesNumber, tmpInt ).good() )
  {

    header.addAttribute( "series_number", int32_t( tmpInt ) );

  }

  if ( dataset.findAndGetOFString( DCM_SeriesDescription, tmpString ).good() )
  {

    header.addAttribute( "series_description", tmpString.c_str() );

  }

  if ( dataset.findAndGetOFString( DCM_Modality, tmpString ).good() )
  {

    header.addAttribute( "modality", tmpString.c_str() );

  }

  if ( dataset.findAndGetOFString( DCM_InstitutionName, tmpString ).good() )
  {

    header.addAttribute( "institution_name", tmpString.c_str() );

  }
  
  int32_t i, n = datasetHeader.size();
  std::vector< std::string > acquisitionTimes;
  std::vector< std::string > contentTimes;

  for ( i = 0; i < n; i++ )
  {

    datasetHeader.get( dataset, i );

    if ( dataset.findAndGetOFString( DCM_AcquisitionTime, tmpString ).good() )
    {

      acquisitionTimes.push_back( tmpString.c_str() );

    }

    if ( dataset.findAndGetOFString( DCM_ContentTime, tmpString ).good() )
    {

      contentTimes.push_back( tmpString.c_str() );

    }

  }

  if ( acquisitionTimes.size() )
  {

    header.addAttribute( "acquisition_time", acquisitionTimes );

  }

  if ( contentTimes.size() )
  {

    header.addAttribute( "content_time", contentTimes );

  }

  return true;

}


bool soma::DicomReader::read( const std::vector< std::string >& fileList, 
                              soma::DicomProxy& proxy, 
                              soma::Callback* progress )
{

  if ( progress )
  {

    progress->execute( 0 );

  }

  if ( !fileList.empty() )
  {

    m_slices = fileList;

    if ( progress )
    {

      progress->execute( 6 );

    }

    // read datset
    bool status = readData( proxy, progress );

    if ( progress )
    {

      progress->execute( 100 );

    }

    return status;

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

    m_dataInfo->m_width = (int32_t)tmpShort;

  }
  else
  {

    return false;

  }

  if ( dataset->findAndGetUint16( DCM_Rows, tmpShort ).good() )
  {

    m_dataInfo->m_height = (int32_t)tmpShort;

  }
  else
  {

    return false;

  }

  if ( dataset->findAndGetUint16( DCM_BitsAllocated, tmpShort ).good() )
  {

    m_dataInfo->m_depth = int32_t( tmpShort < 8 ? 8 : tmpShort );

  }
  else
  {

    return false;

  }

  if ( dataset->findAndGetUint16( DCM_BitsStored, tmpShort ).good() )
  {

    m_dataInfo->m_bitsStored = int32_t( tmpShort < 8 ? 8 : tmpShort );

  }
  else
  {

    return false;

  }

  if ( dataset->findAndGetOFString( DCM_PhotometricInterpretation, 
                                    tmpString ).good() )
  {

    m_dataInfo->m_monochrome = false;

    if ( !tmpString.compare( "MONOCHROME1" ) || 
         !tmpString.compare( "MONOCHROME2" ) )
    {

      m_dataInfo->m_monochrome = true;

    }

  }
  else
  {

    return false;

  }

  Float64 tmpFloat;
  if ( dataset->findAndGetFloat64( DCM_PixelSpacing, tmpFloat, 0 ).good() )
  {

    m_dataInfo->m_resolution.x = (double)tmpFloat;

  }

  if ( dataset->findAndGetFloat64( DCM_PixelSpacing, tmpFloat, 1 ).good() )
  {

    m_dataInfo->m_resolution.y = (double)tmpFloat;

  }

  if ( dataset->findAndGetFloat64( DCM_SpacingBetweenSlices,
                                   tmpFloat ).good() )
  {

    m_dataInfo->m_spacingBetweenSlices = (double)tmpFloat;

  }
  else
  {

    if ( dataset->findAndGetFloat64( DCM_SliceThickness, tmpFloat ).good() )
    {

      m_dataInfo->m_spacingBetweenSlices = (double)tmpFloat;

    }

  }

  if ( dataset->findAndGetFloat64( DCM_RescaleSlope, tmpFloat ).good() )
  {

    m_dataInfo->m_slope.push_back( (double)tmpFloat );

  }
  else
  {

    m_dataInfo->m_slope.push_back( 1.0 );

  }

  if ( dataset->findAndGetFloat64( DCM_RescaleIntercept, tmpFloat ).good() )
  {

     m_dataInfo->m_intercept.push_back( (double)tmpFloat );

  }
  else
  {

     m_dataInfo->m_intercept.push_back( 0.0 );

  }

  m_dataInfo->m_resolution.z = m_dataInfo->m_spacingBetweenSlices;

  m_dataInfo->m_minimum = ( 1 << m_dataInfo->m_bitsStored ) - 1;
  m_dataInfo->m_maximum = 1 - ( 1 << m_dataInfo->m_bitsStored );

  return readHeader( dataset );

}


std::vector< std::string > soma::DicomReader::sortFiles( 
                                              soma::DirectoryParser& directory )
{

  m_slices.push_back( directory.getSelectedFile() );
  m_dataInfo->m_fileCount = 1;

  return m_slices;

}


bool soma::DicomReader::readHeader( DcmDataset* /* dataset */ )
{

  return true;

}
