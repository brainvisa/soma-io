#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/CTImageStorageReader.h>
#include <soma-io/Dicom/CTModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/CTImageStorageReader.h>
#include <Dicom/CTModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::CTImageStorageReader::CTImageStorageReader()
                          : soma::MultiFileReader()
{
}


std::string soma::CTImageStorageReader::getStorageUID()
{

  return UID_CTImageStorage;

}


bool soma::CTImageStorageReader::getHeader( 
                                       soma::HeaderProxy& proxy, 
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::MultiSliceReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  DcmDataset dataset;
  soma::CTModule ctModule;

  datasetHeader.get( dataset );

  if ( ctModule.parseDataset( &dataset ) )
  {

    proxy.addAttribute( "reconstruction_diameter", 
                        ctModule.getReconstructionDiameter() );

  }

  if ( info._pixelPaddingValue )
  {

    proxy.addAttribute( "pixel_padding_value", info._pixelPaddingValue );

  }

  return true;

}


bool soma::CTImageStorageReader::readData(
                                        soma::DicomDatasetHeader& datasetHeader, 
                                        soma::DicomProxy& proxy )
{

  return soma::MultiFileReader::readData( datasetHeader,
                                          proxy, 
                                          false );

}
