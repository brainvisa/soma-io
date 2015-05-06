#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Philips/PhilipsPTReader.h>
#include <soma-io/Dicom/Philips/PhilipsNuclearModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomIO.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/Philips/PhilipsPTReader.h>
#include <Dicom/Philips/PhilipsNuclearModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/DicomIO.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::PhilipsPTReader::PhilipsPTReader()
                    : dcm::PTImageStorageReader(),
                      dcm::Singleton< dcm::PhilipsPTReader >()
{
}


dcm::PhilipsPTReader::~PhilipsPTReader()
{
}


std::string dcm::PhilipsPTReader::getManufacturerName()
{

  return "Philips Medical Systems";

}


bool dcm::PhilipsPTReader::getHeader( dcm::HeaderProxy& proxy,
                                      dcm::DataInfo& info,
                                      dcm::DicomDatasetHeader& datasetHeader )
{

  if ( !dcm::PTImageStorageReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  DcmDataset dataset;
  dcm::PhilipsNuclearModule nuclearModule;

  datasetHeader.get( dataset );
 
  if ( nuclearModule.parseItem( &dataset ) )
  {

    proxy.addAttribute( "image_unit", nuclearModule.getUnits() );
    proxy.addAttribute( "scale_factor", nuclearModule.getScaleFactor() );

  }

  return true;

}


RegisterDicomReaderFunction( PhilipsPTReader );
