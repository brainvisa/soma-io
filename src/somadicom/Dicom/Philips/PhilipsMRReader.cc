#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Philips/PhilipsMRReader.h>
#include <soma-io/Dicom/Philips/PhilipsDiffusionModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomIO.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/Philips/PhilipsMRReader.h>
#include <Dicom/Philips/PhilipsDiffusionModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/DicomIO.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::PhilipsMRReader::PhilipsMRReader()
                    : dcm::MRImageStorageReader(),
                      dcm::Singleton< dcm::PhilipsMRReader >()
{
}


dcm::PhilipsMRReader::~PhilipsMRReader()
{
}


std::string dcm::PhilipsMRReader::getManufacturerName()
{

  return "Philips Medical Systems";

}


bool dcm::PhilipsMRReader::getHeader( dcm::HeaderProxy& proxy,
                                      dcm::DataInfo& info,
                                      dcm::DicomDatasetHeader& datasetHeader )
{

  if ( !dcm::MRImageStorageReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  if ( !proxy.hasAttribute( "b_values" ) )
  {

    dcm::PhilipsDiffusionModule diffusionModule;

    if ( diffusionModule.parseHeader( datasetHeader ) )
    {

      proxy.addAttribute( "b_values", diffusionModule.getBValues() );
      proxy.addAttribute( "diffusion_directions", 
                          diffusionModule.getDirections() );

    }

  }

  return true;

}


bool dcm::PhilipsMRReader::readHeader( DcmDataset* dataset )
{

  bool status = dcm::MRImageStorageReader::readHeader( dataset );

  if ( status )
  {

    if ( _dataInfo->_repetitionTime < 1e-6 )
    {

      Float32 tmpFloat = 0.0;

      status = false;

      if ( dataset->findAndGetFloat32( DcmTagKey( 0x2005, 0x1030 ), 
                                       tmpFloat, 0 ).good() )
      {

        _dataInfo->_repetitionTime = double( tmpFloat ) / 1000.0;

        status = true;

      }

    }

  }

  return status;

}


RegisterDicomReaderFunction( PhilipsMRReader );
