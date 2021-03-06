#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/SCImageStorageWriter.h>
#include <soma-io/Dicom/DicomIO.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/io/scaledcoding.h>
#else
#include <Dicom/SCImageStorageWriter.h>
#include <Dicom/DicomIO.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <sstream>
#include <iomanip>
#include <ctime>
#include <limits>


dcm::SCImageStorageWriter::SCImageStorageWriter()
                         : dcm::DicomWriter(),
                           dcm::Singleton< dcm::SCImageStorageWriter >()
{
}


std::string dcm::SCImageStorageWriter::getStorageUID()
{

  return UID_SecondaryCaptureImageStorage;

}


bool dcm::SCImageStorageWriter::writeDatasets( const std::string& directoryName,
                                               const std::string& fileName,
                                               dcm::DicomProxy& proxy )
{

  bool status = true;
  char uid[ 100 ];
  DcmDataset dataset;
  dcm::DataInfo& info = proxy.getDataInfo();

  // Search for DICOM extension 
  std::string extension = "";
  std::string fileNameWithoutExt = fileName;

  size_t pos = fileName.rfind( ".dcm" );
  if ( pos == std::string::npos )
  {

    pos = fileName.rfind( ".dic" );

  }

  if ( pos != std::string::npos )
  {

    fileNameWithoutExt = fileName.substr( 0, pos );
    extension = fileName.substr( pos );
      
  }
  
  std::string patientName = fileNameWithoutExt;
  std::string studyID = "1";
  std::string seriesID = "1000";
  struct tm* tms;

  dataset.putAndInsertString( DCM_SOPClassUID, 
                              UID_SecondaryCaptureImageStorage );

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
  dataset.putAndInsertString( DCM_PatientName, patientName.c_str() );
  dataset.insertEmptyElement( DCM_PatientBirthDate );
  dataset.insertEmptyElement( DCM_PatientSex );
  dataset.insertEmptyElement( DCM_ReferringPhysicianName );
#else
  dataset.putAndInsertString( DCM_PatientsName, patientName.c_str() );
  dataset.insertEmptyElement( DCM_PatientsBirthDate );
  dataset.insertEmptyElement( DCM_PatientsSex );
  dataset.insertEmptyElement( DCM_ReferringPhysiciansName );
#endif
  dataset.insertEmptyElement( DCM_PatientID );

  dataset.putAndInsertString( DCM_Modality,
                               dcmSOPClassUIDToModality(
                                           UID_SecondaryCaptureImageStorage ) );
  dataset.putAndInsertString( DCM_StudyInstanceUID, 
                              dcmGenerateUniqueIdentifier(
                                                        uid,
                                                        SITE_STUDY_UID_ROOT ) );
  dataset.putAndInsertString( DCM_SeriesInstanceUID, 
                              dcmGenerateUniqueIdentifier(
                                                       uid,
                                                       SITE_SERIES_UID_ROOT ) );

  dataset.putAndInsertString( DCM_StudyID, studyID.c_str() );
  dataset.putAndInsertString( DCM_SeriesNumber, seriesID.c_str() );
  dataset.putAndInsertString( DCM_ConversionType, "WSD" );
  time_t currentTime = std::time( 0 );
  tms = std::localtime( &currentTime );
  std::ostringstream oss;

  oss << 1900 + tms->tm_year;
  oss << std::setfill( '0' ) << std::setw( 2 ) << tms->tm_mon;
  oss << std::setfill( '0' ) << std::setw( 2 ) << tms->tm_mday;
  dataset.putAndInsertString( DCM_StudyDate, oss.str().c_str() );

  oss.str( "" );
  oss << std::setfill( '0' ) << std::setw( 2 ) << tms->tm_hour;
  oss << std::setfill( '0' ) << std::setw( 2 ) << tms->tm_min;
  oss << std::setfill( '0' ) << std::setw( 2 ) << tms->tm_sec;
  dataset.putAndInsertString( DCM_StudyTime, oss.str().c_str() );

  dataset.putAndInsertUint16( DCM_SamplesPerPixel, info._spp );

  if ( info._spp == 3 )
  {

    dataset.putAndInsertString( DCM_PhotometricInterpretation, "RGB" );

  }
  else if ( info._invertLut )
  {

    dataset.putAndInsertString( DCM_PhotometricInterpretation, "MONOCHROME1" );

  }
  else
  {

    dataset.putAndInsertString( DCM_PhotometricInterpretation, "MONOCHROME2" );

  }

  dataset.putAndInsertUint16( DCM_Rows, info._height );
  dataset.putAndInsertUint16( DCM_Columns, info._width );

  if ( info._modalityLut && ( info._depth == 32 ) )
  {

    dataset.putAndInsertUint16( DCM_BitsAllocated, 16 );
    dataset.putAndInsertUint16( DCM_BitsStored, 16 );
    dataset.putAndInsertUint16( DCM_HighBit, 15 );

  }
  else
  {

    dataset.putAndInsertUint16( DCM_BitsAllocated, info._depth );
    dataset.putAndInsertUint16( DCM_BitsStored, info._depth );
    dataset.putAndInsertUint16( DCM_HighBit, info._depth - 1 );

  }
  dataset.putAndInsertUint16( DCM_PixelRepresentation,
                              info._pixelRepresentation );

  oss.str( "" );
  oss << info._resolution.x;

  std::string tmpStr = oss.str();

  if ( tmpStr.length() > 16 )
  {

    tmpStr.resize( 16 );

  }

  std::string pixelSpacing = tmpStr;

  oss.str( "" );
  oss << info._resolution.y;
  tmpStr = oss.str();

  if ( tmpStr.length() > 16 )
  {

    tmpStr.resize( 16 );

  }

  pixelSpacing += "\\" + tmpStr;
  dataset.putAndInsertString( DCM_PixelSpacing, pixelSpacing.c_str() );

  int32_t z, dimZ = info._slices;
  int32_t t, dimT = info._frames;
  int32_t i, dataCount = info._sliceSize * info._bpp;

  if ( dimZ > 1 )
  {

    oss.str( "" );
    oss << info._resolution.z;
    tmpStr = oss.str();

    if ( tmpStr.length() > 16 )
    {

      tmpStr.resize( 16 );

    }

    dataset.putAndInsertString( DCM_SliceThickness, tmpStr.c_str() );

  }

  dataset.putAndInsertString( DCM_ImageOrientationPatient, 
                              "1\\0\\0\\0\\1\\0" );

  float sliceLocation;
  std::vector< long > strides( 4 );
  std::vector< int32_t > sizes( 4 );
  strides[ 0 ] = 1;
  strides[ 1 ] = info._width;
  strides[ 2 ] = info._sliceSize;
  strides[ 3 ] = info._volumeSize;
  sizes[ 0 ] = info._width;
  sizes[ 1 ] = info._height;
  sizes[ 2 ] = 1;
  sizes[ 3 ] = 1;

  for ( i = 1, t = 0; status && ( t < dimT ); t++ )
  {

    sliceLocation = 0.0f;

    for ( z = 0; status && ( z < dimZ ); z++, i++ )
    {

      std::string instanceUID = dcmGenerateUniqueIdentifier(
                                                       uid,
                                                       SITE_INSTANCE_UID_ROOT );

      dataset.putAndInsertString( DCM_SOPInstanceUID, instanceUID.c_str() );

      oss.str( "" );
      oss << i;
      dataset.putAndInsertString( DCM_InstanceNumber, oss.str().c_str() );

      oss.str( "" );
      oss << sliceLocation;

      std::string strLocation = oss.str();

      if ( strLocation.length() > 16 )
      {

        strLocation.resize( 16 );

      }

      tmpStr = "0\\0\\" + strLocation;
      dataset.putAndInsertString( DCM_SliceLocation, strLocation.c_str() );
      dataset.putAndInsertString( DCM_ImagePositionPatient, tmpStr.c_str() );

      if ( info._modalityLut && ( info._depth == 32 ) )
      {

        uint16_t buffer[ info._sliceSize ];
        soma::ScaledEncodingInfo sinfo = 
          soma::ScaledEncoding< float, uint16_t >::rescale( 
                                                    (float*)proxy( 0, 0, z, t ),
                                                    strides,
                                                    sizes, 
                                                    buffer );

        oss.str( "" );
        oss << sinfo.slope();
        dataset.putAndInsertString( DCM_RescaleSlope, oss.str().c_str() );
        oss.str( "" );
        oss << sinfo.offset() - 
               sinfo.slope() * (double)std::numeric_limits<uint16_t>::min();
        dataset.putAndInsertString( DCM_RescaleIntercept, oss.str().c_str() );
        dataset.putAndInsertUint16Array( DCM_PixelData,
                                         buffer,
                                         (unsigned long)info._sliceSize );

      }
      else if ( info._depth > 8 )
      {

        dataset.putAndInsertUint16Array( DCM_PixelData,
                                         (Uint16*)proxy( 0, 0, z, t ),
                                         (unsigned long)info._sliceSize );

      }
      else
      {

        dataset.putAndInsertUint8Array( DCM_PixelData,
                                        (Uint8*)proxy( 0, 0, z, t ),
                                        (unsigned long)dataCount );

      }

      std::string fileName = directoryName + patientName + "_" +
                             studyID + "_" + seriesID + "_" + instanceUID +
                             extension;

      DcmFileFormat fileFormat( &dataset );
      if ( fileFormat.saveFile( fileName.c_str(), 
                                EXS_LittleEndianExplicit ).bad() )
      {

        status = false;

      }

      sliceLocation -= info._resolution.z;

    }

  }

  return status;

}


RegisterDicomWriterFunction( SCImageStorageWriter );
