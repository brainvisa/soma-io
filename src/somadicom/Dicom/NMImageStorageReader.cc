#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/NMImageStorageReader.h>
#include <soma-io/Dicom/PatientModule.h>
#include <soma-io/Dicom/OrientationModule.h>
#include <soma-io/Dicom/PositionModule.h>
#include <soma-io/Dicom/AcquisitionModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/NMImageStorageReader.h>
#include <Dicom/PatientModule.h>
#include <Dicom/OrientationModule.h>
#include <Dicom/PositionModule.h>
#include <Dicom/AcquisitionModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::NMImageStorageReader::NMImageStorageReader()
                         : dcm::SingleFileReader(),
                           dcm::MultiSliceReader()
{
}


std::string dcm::NMImageStorageReader::getStorageUID()
{

  return UID_NuclearMedicineImageStorage;

}


bool dcm::NMImageStorageReader::getHeader(
                                        dcm::HeaderProxy& header,
                                        dcm::DataInfo& info,
                                        dcm::DicomDatasetHeader& datasetHeader )
{

  if ( !dcm::SingleFileReader::getHeader( header, info, datasetHeader ) )
  {

    return false;

  }

  if ( !dcm::MultiSliceReader::getHeader( header, info, datasetHeader ) )
  {

    return false;

  }

  header.addAttribute( "number_of_frames", info._slices );

  OFString tmpString;
  DcmDataset dataset;
  dcm::PatientModule patientModule;

  datasetHeader.get( dataset );

  if ( patientModule.parseItem( &dataset ) )
  {

    header.addAttribute( "patient_birthdate", patientModule.getBirthDate() );
    header.addAttribute( "patient_sex", patientModule.getSex() );

  }

  if ( dataset.findAndGetOFString( DCM_CorrectedImage, tmpString ).good() )
  {

    header.addAttribute( "corrected_image", tmpString.c_str() );

  }

  return true;

}


bool dcm::NMImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    _dataInfo->_fileCount = 1;
    _dataInfo->_resolution.z = _dataInfo->_spacingBetweenSlices;
    _positions.clear();

    Uint16 nSlices = 1;

    if ( dataset->findAndGetUint16( DCM_NumberOfSlices, nSlices ).good() )
    {

      _dataInfo->_slices = (int32_t)nSlices;

    }
    else if ( dataset->findAndGetUint16( DCM_NumberOfFrames, nSlices ).good() )
    {

      _dataInfo->_slices = (int32_t)nSlices;

    }

    DcmItem* item = 0;
    if ( dataset->findAndGetSequenceItem( DCM_DetectorInformationSequence,
                                          item ).good() )
    {

      dcm::OrientationModule orientationModule;
      dcm::PositionModule positionModule;

      if ( orientationModule.parseItem( item ) )
      {

        _dataInfo->_rowVec = orientationModule.getRowCosine();
        _dataInfo->_colVec = orientationModule.getColumnCosine();

      }

      if ( positionModule.parseItem( item ) )
      {

        _dataInfo->_origin = positionModule.getImagePosition();

      }

    }

    return dcm::SingleFileReader::readHeader( dataset );

  }

  return false;

}


bool dcm::NMImageStorageReader::buildIndexLut( DcmDataset* dataset )
{

  dcm::AcquisitionModule acquisitionModule;
  int32_t i, n = _dataInfo->_slices * _dataInfo->_frames;

  _indexLut.resize( n );

  if ( acquisitionModule.parseItem( dataset ) &&
       ( acquisitionModule.getSpacingBetweenSlices() < 0 ) )
  {

    for ( i = 0; i < n; i++ )
    {

      _indexLut[ i ] = n - i - 1;

    }

  }
  else
  {

    for ( i = 0; i < n; i++ )
    {

      _indexLut[ i ] = i;

    }

  }

  return true;

}
