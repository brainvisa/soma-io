#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/NMImageStorageReader.h>
#include <soma-io/Dicom/PatientModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/NMImageStorageReader.h>
#include <Dicom/PatientModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::NMImageStorageReader::NMImageStorageReader()
                          : soma::MultiSliceReader()
{
}


std::string soma::NMImageStorageReader::getStorageUID()
{

  return UID_NuclearMedicineImageStorage;

}


bool soma::NMImageStorageReader::getHeader(
                                       soma::HeaderProxy& header,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::MultiSliceReader::getHeader( header, info, datasetHeader ) )
  {

    return false;

  }

  header.addAttribute( "number_of_frames", info._slices );

  OFString tmpString;
  DcmDataset dataset;
  soma::PatientModule patientModule;

  datasetHeader.get( dataset );

  if ( patientModule.parseDataset( &dataset ) )
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


bool soma::NMImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    _dataInfo->_fileCount = 1;
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

      OFString tmpString;

      if ( item->findAndGetOFStringArray( DCM_ImageOrientationPatient,
                                          tmpString ).good() )
      {

        std::string orientationStr = tmpString.c_str();
        size_t pos = orientationStr.find( "\\" );

        while ( pos != std::string::npos )
        {

          orientationStr[ pos ] = ' ';
          pos = orientationStr.find( "\\" );

        }

        std::istringstream iss( orientationStr );

        iss >> _dataInfo->_rowVec.x
            >> _dataInfo->_rowVec.y
            >> _dataInfo->_rowVec.z
            >> _dataInfo->_colVec.x
            >> _dataInfo->_colVec.y
            >> _dataInfo->_colVec.z;

      }

      if ( item->findAndGetOFStringArray( DCM_ImagePositionPatient,
                                          tmpString ).good() )
      {

        std::string positionStr = tmpString.c_str();
        size_t pos = positionStr.find( "\\" );

        while ( pos != std::string::npos )
        {

          positionStr[ pos ] = ' ';
          pos = positionStr.find( "\\" );

        }

        soma::Vector position;
        std::istringstream iss( positionStr );

        iss >> position.x
            >> position.y
            >> position.z;

        _positions.push_back( position );

      }

    }

    return soma::MultiSliceReader::readHeader( dataset );

  }

  return false;

}
