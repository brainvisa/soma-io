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


soma::MRImageStorageReader::MRImageStorageReader()
                          : soma::MultiFileReader()
{
}


std::string soma::MRImageStorageReader::getStorageUID()
{

  return UID_MRImageStorage;

}


bool soma::MRImageStorageReader::getHeader(
                                       soma::HeaderProxy& proxy,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::MultiSliceReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  DcmDataset dataset;
  soma::MRModule mrModule;
  soma::DiffusionModule diffusionModule;

  datasetHeader.get( dataset );

  if ( mrModule.parseDataset( &dataset ) )
  {

    proxy.addAttribute( "tr", mrModule.getTR() );
    proxy.addAttribute( "te", mrModule.getTE() );
    proxy.addAttribute( "ti", mrModule.getTI() );
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


bool soma::MRImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    Float64 tmpFloat;

    if ( dataset->findAndGetFloat64( DCM_RepetitionTime, tmpFloat ).good() )
    {

      _dataInfo->_repetitionTime = (double)tmpFloat / 1000.0;

    }

    return soma::MultiFileReader::readHeader( dataset );

  }

  return false;

}
