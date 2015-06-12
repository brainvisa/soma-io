#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MRImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/MRModule.h>
#include <soma-io/Dicom/DiffusionModule.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/MRImageStorageReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/MRModule.h>
#include <Dicom/DiffusionModule.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::MRImageStorageReader::MRImageStorageReader()
                         : dcm::MultiFileReader()
{
}


std::string dcm::MRImageStorageReader::getStorageUID()
{

  return UID_MRImageStorage;

}


bool dcm::MRImageStorageReader::getHeader(
                                        dcm::HeaderProxy& proxy,
                                        dcm::DataInfo& info,
                                        dcm::DicomDatasetHeader& datasetHeader )
{

  if ( !dcm::MultiSliceReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  dcm::MRModule mrModule;
  dcm::DiffusionModule diffusionModule;

  if ( mrModule.parseHeader( datasetHeader ) )
  {

    proxy.addAttribute( "tr", mrModule.getTR() );
    proxy.addAttribute( "TEs", mrModule.getTE() );
    proxy.addAttribute( "te", mrModule.getTE()[ 0 ] );

    if ( mrModule.getTI().size() )
    {

      proxy.addAttribute( "TIs", mrModule.getTI() );
      proxy.addAttribute( "ti", mrModule.getTI()[ 0 ] );

    }

    proxy.addAttribute( "flip_angle", mrModule.getFlipAngle() );

  }

  if ( diffusionModule.parseHeader( datasetHeader ) )
  {

    proxy.addAttribute( "b_values", diffusionModule.getBValues() );
    proxy.addAttribute( "diffusion_directions", 
                        diffusionModule.getDirections() );

  }

  return true;

}


bool dcm::MRImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    Float64 tmpFloat = 0.0;

    if ( dataset->findAndGetFloat64( DCM_RepetitionTime, tmpFloat ).good() )
    {

      _dataInfo->_repetitionTime = (double)tmpFloat / 1000.0;

    }

    return dcm::MultiFileReader::readHeader( dataset );

  }

  return false;

}
