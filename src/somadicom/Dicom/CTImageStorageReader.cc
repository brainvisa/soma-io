#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/CTImageStorageReader.h>
#include <soma-io/Dicom/CTModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DataInfo.h>
#include <soma-io/Object/HeaderProxy.h>
#else
#include <Dicom/CTImageStorageReader.h>
#include <Dicom/CTModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DataInfo.h>
#include <Object/HeaderProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::CTImageStorageReader::CTImageStorageReader()
                         : dcm::MultiFileReader()
{
}


std::string dcm::CTImageStorageReader::getStorageUID()
{

  return UID_CTImageStorage;

}


bool dcm::CTImageStorageReader::getHeader( 
                                        dcm::HeaderProxy& proxy, 
                                        dcm::DataInfo& info,
                                        dcm::DicomDatasetHeader& datasetHeader )
{

  if ( !dcm::MultiFileReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  DcmDataset dataset;
  dcm::CTModule ctModule;

  datasetHeader.get( dataset );

  if ( ctModule.parseItem( &dataset ) )
  {

    proxy.addAttribute( "reconstruction_diameter", 
                        ctModule.getReconstructionDiameter() );

  }

  return true;

}
