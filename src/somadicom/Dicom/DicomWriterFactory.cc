#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomWriterFactory.h>
#include <soma-io/Dicom/DicomWriter.h>
#include <soma-io/Dicom/DatasetModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/DicomWriterFactory.h>
#include <Dicom/DicomWriter.h>
#include <Dicom/DatasetModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <iostream>


dcm::DicomWriterFactory::DicomWriterFactory()
{
}


dcm::DicomWriterFactory::~DicomWriterFactory()
{
}


bool dcm::DicomWriterFactory::add( dcm::DicomWriter* writer )
{

  if ( writer )
  {

    std::string storageUID = writer->getStorageUID();
    std::map< std::string, dcm::DicomWriter* >::iterator
      w = _writers.find( storageUID );

    if ( w == _writers.end() )
    {

      _writers.insert( std::make_pair( storageUID, writer ) );

      return true;

    }

  }

  return false;

}


bool dcm::DicomWriterFactory::write( const std::string& fileName,
                                     dcm::DicomProxy& proxy,
                                     bool forceSecondaryCapture )
{

  std::string storageUID;
  std::string defaultStorageUID = UID_SecondaryCaptureImageStorage;

  if ( !forceSecondaryCapture && 
       ( proxy.getBinaryHeader().getFormat() == "dicom" ) )
  {

    DcmDataset dataset;
    dcm::DatasetModule datasetModule;
    dcm::DicomDatasetHeader datasetHeader( proxy );

    datasetHeader.get( dataset );

    if ( datasetModule.parseItem( &dataset ) )
    {

      storageUID = datasetModule.getSOPClassUID();

    }

  }

  std::map< std::string, dcm::DicomWriter* >::const_iterator
    w = _writers.find( storageUID );

  if ( w == _writers.end() )
  {

    w = _writers.find( defaultStorageUID );

  }

  if ( w != _writers.end() )
  {

    return w->second->write( fileName, proxy );

  }

  return false;

}
