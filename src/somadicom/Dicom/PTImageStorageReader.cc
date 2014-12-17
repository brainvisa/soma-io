#include <soma-io/Dicom/PTImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/DicomDataContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::PTImageStorageReader::PTImageStorageReader()
                          : soma::MultiFileReader()
{

  // from DICOM 3 - Part 3 - C.8.9.1.1.3
  m_unitNames.insert( std::make_pair( "CNTS", "counts" ) );
  m_unitNames.insert( std::make_pair( "NONE", "unitless" ) );
  m_unitNames.insert( std::make_pair( "CM2", "cm2" ) );
  m_unitNames.insert( std::make_pair( "PCNT", "percent" ) );
  m_unitNames.insert( std::make_pair( "CPS", "counts/s" ) );
  m_unitNames.insert( std::make_pair( "BQML", "Bq/ml" ) );
  m_unitNames.insert( std::make_pair( "MGMINML", "mg/min/ml" ) );
  m_unitNames.insert( std::make_pair( "UMOLMINML", "uMol/min/ml" ) );
  m_unitNames.insert( std::make_pair( "MLMING", "ml/min/g" ) );
  m_unitNames.insert( std::make_pair( "MLG", "ml/g" ) );
  m_unitNames.insert( std::make_pair( "1CM", "1/cm" ) );
  m_unitNames.insert( std::make_pair( "UMOLML", "uMol/ml" ) );
  m_unitNames.insert( std::make_pair( "PROPCNTS", "proportional to counts" ) );
  m_unitNames.insert( std::make_pair( "PROPCPS", "proportional to counts/s" ) );
  m_unitNames.insert( std::make_pair( "MLMINML", "ml/min/ml" ) );
  m_unitNames.insert( std::make_pair( "MLML", "ml/ml" ) );
  m_unitNames.insert( std::make_pair( "GML", "g/ml" ) );
  m_unitNames.insert( std::make_pair( "STDDEV", "standard deviations" ) );

}


std::string soma::PTImageStorageReader::getStorageUID()
{

#if OFFIS_DCMTK_VERSION_NUMBER >= 360

  return UID_PositronEmissionTomographyImageStorage;

#else

  return UID_PETImageStorage;

#endif

}


bool soma::PTImageStorageReader::getHeader( soma::HeaderProxy& header, 
                                            soma::DataInfo& info )
{

  if ( !soma::MultiSliceReader::getHeader( header, info ) )
  {

    return false;

  }

  OFString tmpString;
  Float64 tmpDouble;
  DcmDataset dataset;
  soma::DicomDatasetHeader datasetHeader( info );

  datasetHeader.get( dataset );

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
  if ( dataset.findAndGetOFString( DCM_PatientSex, tmpString ).good() )
#else
  if ( dataset.findAndGetOFString( DCM_PatientsSex, tmpString ).good() )
#endif
  {

    header.addAttribute( "patient_sex", tmpString.c_str() );

  }

  if ( dataset.findAndGetOFString( DCM_Units, tmpString ).good() )
  {

    header.addAttribute( "image_unit", m_unitNames[ tmpString.c_str() ] );

  }

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
  if ( dataset.findAndGetOFString( DCM_PatientAge, tmpString ).good() )
#else
  if ( dataset.findAndGetOFString( DCM_PatientsAge, tmpString ).good() )
#endif
  {

    header.addAttribute( "patient_age", tmpString.c_str() );

  }

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
  if ( dataset.findAndGetFloat64( DCM_PatientWeight, tmpDouble ).good() )
#else
  if ( dataset.findAndGetFloat64( DCM_PatientsWeight, tmpDouble ).good() )
#endif
  {

    header.addAttribute( "patient_weight", double( tmpDouble ) );

  }

  if ( dataset.findAndGetOFString( DCM_SeriesDate, tmpString ).good() )
  {

    header.addAttribute( "series_date", tmpString.c_str() );

  }

  if ( dataset.findAndGetOFString( DCM_SeriesTime, tmpString ).good() )
  {

    header.addAttribute( "series_time", tmpString.c_str() );

  }

  return true;

}


bool soma::PTImageStorageReader::readData( soma::DicomProxy& proxy,
                                           soma::Callback* progress )
{

  if ( proxy.allocate() )
  {

    soma::DicomDataContext context( m_slices, proxy, false, progress );
    carto::ThreadedLoop threadedLoop( &context,
                                      0,
                                      int32_t( m_slices.size() ) );

    threadedLoop.launch();

    return true;

  }

  return false;

}
