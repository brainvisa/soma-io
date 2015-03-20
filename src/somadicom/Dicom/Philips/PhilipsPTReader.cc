#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Philips/PhilipsPTReader.h>
#include <soma-io/Dicom/Philips/PhilipsNuclearModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/Philips/PhilipsPTReader.h>
#include <Dicom/Philips/PhilipsNuclearModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/DicomReaderFactory.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::PhilipsPTReader::PhilipsPTReader()
                     : soma::PTImageStorageReader(),
                       soma::Singleton< soma::PhilipsPTReader >()
{
}


soma::PhilipsPTReader::~PhilipsPTReader()
{
}


std::string soma::PhilipsPTReader::getManufacturerName()
{

  return "Philips Medical Systems";

}


bool soma::PhilipsPTReader::getHeader( soma::HeaderProxy& proxy,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::PTImageStorageReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  DcmDataset dataset;
  soma::PhilipsNuclearModule nuclearModule;

  datasetHeader.get( dataset );
 
  if ( nuclearModule.parseItem( &dataset ) )
  {

    proxy.addAttribute( "image_unit", nuclearModule.getUnits() );
    proxy.addAttribute( "scale_factor", nuclearModule.getScaleFactor() );

  }

  return true;

}


RegisterDicomReaderFunction( PhilipsPTReader );

