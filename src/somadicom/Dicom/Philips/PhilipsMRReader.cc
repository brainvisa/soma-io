#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Philips/PhilipsMRReader.h>
#include <soma-io/Dicom/Philips/PhilipsDiffusionModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/Philips/PhilipsMRReader.h>
#include <Dicom/Philips/PhilipsDiffusionModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/DicomReaderFactory.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::PhilipsMRReader::PhilipsMRReader()
                     : soma::MRImageStorageReader(),
                       soma::Singleton< soma::PhilipsMRReader >()
{
}


soma::PhilipsMRReader::~PhilipsMRReader()
{
}


std::string soma::PhilipsMRReader::getManufacturerName()
{

  return "Philips Medical Systems";

}


bool soma::PhilipsMRReader::getHeader( soma::HeaderProxy& proxy,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::MRImageStorageReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  if ( !proxy.hasAttribute( "b_values" ) )
  {

    soma::PhilipsDiffusionModule diffusionModule;

    if ( diffusionModule.parseHeader( datasetHeader ) )
    {

      proxy.addAttribute( "b_values", diffusionModule.getBValues() );
      proxy.addAttribute( "diffusion_directions", 
                          diffusionModule.getDirections() );

    }

  }

  return true;

}


RegisterDicomReaderFunction( PhilipsMRReader );

