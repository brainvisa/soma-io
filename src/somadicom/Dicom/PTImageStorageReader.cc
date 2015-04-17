#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/PTImageStorageReader.h>
#include <soma-io/Dicom/PatientModule.h>
#include <soma-io/Dicom/NuclearModule.h>
#include <soma-io/Dicom/RadiopharmaceuticalModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/PTImageStorageReader.h>
#include <Dicom/PatientModule.h>
#include <Dicom/NuclearModule.h>
#include <Dicom/RadiopharmaceuticalModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::PTImageStorageReader::PTImageStorageReader()
                         : dcm::MultiFileReader()
{
}


std::string dcm::PTImageStorageReader::getStorageUID()
{

#if OFFIS_DCMTK_VERSION_NUMBER >= 360

  return UID_PositronEmissionTomographyImageStorage;

#else

  return UID_PETImageStorage;

#endif

}


bool dcm::PTImageStorageReader::getHeader( 
                                        dcm::HeaderProxy& header, 
                                        dcm::DataInfo& info,
                                        dcm::DicomDatasetHeader& datasetHeader )
{

  if ( !dcm::MultiSliceReader::getHeader( header, info, datasetHeader ) )
  {

    return false;

  }

  OFString tmpString;
  Float64 tmpDouble;
  Sint32 tmpInt;
  DcmDataset dataset;
  dcm::PatientModule patientModule;
  dcm::NuclearModule nuclearModule;
  dcm::RadiopharmaceuticalModule radiopharmaModule;

  datasetHeader.get( dataset );

  if ( patientModule.parseItem( &dataset ) )
  {

    header.addAttribute( "patient_sex", patientModule.getSex() );
    header.addAttribute( "patient_age", patientModule.getAge() );
    header.addAttribute( "patient_weight", patientModule.getWeight() );

  }

  std::string process_list = "";

  if ( dataset.findAndGetOFStringArray( DCM_CorrectedImage, tmpString ).good() )
  {

    process_list = tmpString.c_str();

  }

  if ( nuclearModule.parseItem( &dataset ) )
  {

    const std::string& mode = nuclearModule.getAcquisitionMode();

    if ( mode == "DYNAMIC" )
    {

      int32_t nbGates = nuclearModule.getNumberOfTimeSlots();

      if ( nbGates )
      {

        header.addAttribute( "nb_gates", nbGates );

      }

      header.addAttribute( "nb_t_pos", nuclearModule.getNumberOfTimeSlices() );

    }

    header.addAttribute( "acquisition_mode", mode );
    header.addAttribute( "category", nuclearModule.getCategory() );
    header.addAttribute( "image_unit", nuclearModule.getUnits() );
    header.addAttribute( "reconstruction_method", 
                         nuclearModule.getReconstructionMethod() );

    const std::string& correction = nuclearModule.getCorrectionMethod();

    if ( correction.size() )
    {

      if ( process_list.size() )
      {

        process_list += "\\";

      }

      process_list += correction;

    }

  }

  if ( process_list.size() )
  {

    header.addAttribute( "process_list", process_list );

  }

  if ( radiopharmaModule.parseItem( &dataset ) )
  {

    header.addAttribute( "radiopharmaceutical", 
                         radiopharmaModule.getRadiopharmaceutical() );
    header.addAttribute( "radiopharmaceutical_start_time", 
                         radiopharmaModule.getRadiopharmaceuticalStartTime() );
    header.addAttribute( "radionuclide_total_dose",
                         radiopharmaModule.getRadionuclideTotalDose() );
    header.addAttribute( "radionuclide_half_life",
                         radiopharmaModule.getRadionuclideHalfLife() );

    double positronFraction = 
                            radiopharmaModule.getRadionuclidePositronFraction();

    if ( positronFraction > 0.0 )
    {

      header.addAttribute( "radionuclide_positron_fraction", positronFraction );

    }

    header.addAttribute( "radionuclide", 
                         radiopharmaModule.getRadionuclide() );

  }

  if ( dataset.findAndGetFloat64( DCM_TriggerTime,
                                  tmpDouble ).good() )
  {

    header.addAttribute( "gate_time", int32_t( tmpDouble ) );

  }

  int32_t i, k = 0, n = datasetHeader.size();
  int32_t sizeT = info._frames;
  std::vector< int32_t > start_time( sizeT, 0 );
  std::vector< int32_t > duration_time( sizeT, 0 );
  dcm::Vector3d< int32_t > sizes = info._patientOrientation.getOnDiskSize();
  int32_t step = sizes.z;

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
