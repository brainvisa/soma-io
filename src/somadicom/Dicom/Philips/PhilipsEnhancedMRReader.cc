#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Philips/PhilipsEnhancedMRReader.h>
#include <soma-io/Dicom/Philips/PhilipsEnhancedDiffusionModule.h>
#include <soma-io/Dicom/Philips/PhilipsEnhancedIndexModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/Philips/PhilipsEnhancedMRReader.h>
#include <Dicom/Philips/PhilipsEnhancedDiffusionModule.h>
#include <Dicom/Philips/PhilipsEnhancedIndexModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/DicomReaderFactory.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::PhilipsEnhancedMRReader::PhilipsEnhancedMRReader()
                             : soma::EnhancedMRImageStorageReader(),
                               soma::Singleton< 
                                              soma::PhilipsEnhancedMRReader >(),
                               _tIndex( 0 )
{
}


soma::PhilipsEnhancedMRReader::~PhilipsEnhancedMRReader()
{
}


std::string soma::PhilipsEnhancedMRReader::getManufacturerName()
{

  return "Philips Medical Systems";

}


bool soma::PhilipsEnhancedMRReader::getHeader( 
                                       soma::HeaderProxy& proxy,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::EnhancedMRImageStorageReader::getHeader( proxy, 
                                                       info, 
                                                       datasetHeader ) )
  {

    return false;

  }

  if ( !proxy.hasAttribute( "b_values" ) )
  {

    soma::PhilipsEnhancedDiffusionModule diffusionModule;

    if ( diffusionModule.parseHeader( datasetHeader ) )
    {

      proxy.addAttribute( "b_values", diffusionModule.getBValues() );
      proxy.addAttribute( "diffusion_directions", 
                          diffusionModule.getDirections() );

    }

  }

  return true;

}


bool soma::PhilipsEnhancedMRReader::buildIndexLut( DcmDataset* dataset )
{

  if ( dataset )
  {

    soma::PhilipsEnhancedIndexModule indexModule;

    if ( indexModule.parseItem( dataset ) )
    {

      _dataInfo->_slices = indexModule.getZCount();
      _dataInfo->_frames = indexModule.getTCount();
      _tIndex = indexModule.getTIndex();
      _indexLut = indexModule.getIndices();

      return true;

    }

  }

  return false;

}


RegisterDicomReaderFunction( PhilipsEnhancedMRReader );

