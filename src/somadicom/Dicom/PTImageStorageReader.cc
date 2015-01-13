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


bool soma::PTImageStorageReader::getHeader( 
                                       soma::HeaderProxy& header, 
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::MultiSliceReader::getHeader( header, info, datasetHeader ) )
  {

    return false;

  }

  OFString tmpString;
  Float64 tmpDouble;
  Sint32 tmpInt;
  Uint32 tmpUint;
  DcmDataset dataset;

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

  if ( dataset.findAndGetSint32( DCM_NumberOfTemporalPositions,
                                 tmpInt ).good() )
  {

    header.addAttribute( "nb_t_pos", int32_t( tmpInt ) );

  }

  if ( dataset.findAndGetSint32( DCM_AcquisitionNumber, tmpInt ).good() )
  {

    header.addAttribute( "acquisition_number", int32_t( tmpInt ) );

  }

  DcmItem* item = 0;
  if ( dataset.findAndGetSequenceItem( 
                                     DCM_RadiopharmaceuticalInformationSequence,
                                     item ).good() )
  {

    DcmItem* rnItem = 0;

    if ( item->findAndGetSequenceItem( DCM_RadionuclideCodeSequence,
                                       rnItem ).good() )
    {

      if ( rnItem->findAndGetOFString( DCM_CodeMeaning, tmpString ).good() )
      {

        header.addAttribute( "radionuclide", tmpString.c_str() );

      }

    }

    if ( item->findAndGetOFString( DCM_RadiopharmaceuticalStartTime,
                                   tmpString ).good() )
    {

      header.addAttribute( "radiopharmaceutical_start_time", 
                           tmpString.c_str() );

    }

    if ( item->findAndGetFloat64( DCM_RadionuclideTotalDose,
                                  tmpDouble ).good() )
    {

      header.addAttribute( "radionuclide_total_dose", double( tmpDouble ) );

    }

    if ( item->findAndGetFloat64( DCM_RadionuclideHalfLife,
                                  tmpDouble ).good() )
    {

      header.addAttribute( "radionuclide_half_life", double( tmpDouble ) );

    }

    if ( item->findAndGetFloat64( DCM_RadionuclidePositronFraction,
                                  tmpDouble ).good() )
    {

      header.addAttribute( "radionuclide_positron_fraction", double( tmpDouble ) );

    }

  }

  if ( dataset.findAndGetFloat64( DCM_TriggerTime,
                                  tmpDouble ).good() )
  {

    header.addAttribute( "gate_time", int32_t( tmpDouble ) );

  }

  if ( dataset.findAndGetOFString( DCM_ReconstructionMethod,
                                   tmpString ).good() )
  {

    header.addAttribute( "reconstruction_method", tmpString.c_str() );

  }

  std::string process_list = "";

  if ( dataset.findAndGetOFStringArray( DCM_CorrectedImage, tmpString ).good() )
  {

    process_list = tmpString.c_str();

  }

  if ( dataset.findAndGetOFString( DCM_RandomsCorrectionMethod, 
                                   tmpString ).good() )
  {

    if ( process_list.size() )
    {

      process_list += "\\";

    }

    process_list += tmpString.c_str();

  }

  if ( dataset.findAndGetOFString( DCM_AttenuationCorrectionMethod, 
                                   tmpString ).good() )
  {

    if ( process_list.size() )
    {

      process_list += "\\";

    }

    process_list += tmpString.c_str();

  }

  if ( dataset.findAndGetOFString( DCM_ScatterCorrectionMethod, 
                                   tmpString ).good() )
  {

    if ( process_list.size() )
    {

      process_list += "\\";

    }

    process_list += tmpString.c_str();

  }

  if ( process_list.size() )
  {

    header.addAttribute( "process_list", process_list );

  }

  if ( dataset.findAndGetOFString( DCM_SeriesType, tmpString, 0 ).good() )
  {

    header.addAttribute( "acquisition_mode", tmpString.c_str() );

  }

  if ( !tmpString.compare( "DYNAMIC" ) )
  {

    int32_t nb_t_pos = 1;
    Uint16 tmpUint16;

    if ( dataset.findAndGetUint16( DCM_NumberOfTimeSlices, tmpUint16 ).good() )
    {

      nb_t_pos = int32_t( tmpUint16 );

    }

    header.addAttribute( "nb_t_pos", nb_t_pos );

    if ( dataset.findAndGetUint32( DCM_NumberOfTimeSlots, tmpUint ).good() )
    {

      header.addAttribute( "nb_gates", int32_t( tmpUint ) );

    }

  }

  if ( dataset.findAndGetOFString( DCM_SeriesType, tmpString, 1 ).good() )
  {

    header.addAttribute( "category", tmpString.c_str() );

  }

  int32_t i, k = 0, n = datasetHeader.size();
  int32_t dummyX, dummyY, step, sizeT = info.m_frames;
  std::vector< int32_t > start_time( sizeT, 0 );
  std::vector< int32_t > duration_time( sizeT, 0 );

  info.m_patientOrientation.getOnDiskSize( dummyX, dummyY, step );

  for ( i = 0; i < n; i += step )
  {

    datasetHeader.get( dataset, i );

    if ( dataset.findAndGetSint32( DCM_ActualFrameDuration, tmpInt ).good() )
    {

      duration_time[ k++ ] = int32_t( tmpInt / 1000 );

    }

  }

  for ( i = 1; i < sizeT; i++ )
  {

    start_time[ i ] = start_time[ i - 1 ] + duration_time[ i - 1 ];

  }

  header.addAttribute( "start_time", start_time );
  header.addAttribute( "duration_time", duration_time );

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
