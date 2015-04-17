#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Dicom/DicomReader.h>
#include <soma-io/Dicom/DatasetModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/System/DirectoryParser.h>
#else
#include <Dicom/DicomReaderFactory.h>
#include <Dicom/DicomReader.h>
#include <Dicom/DatasetModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#include <System/DirectoryParser.h>
#endif

#include <iostream>


dcm::DicomReaderFactory::DicomReaderFactory()
                       : dcm::Singleton< dcm::DicomReaderFactory >()
{
}


bool dcm::DicomReaderFactory::registerReader( dcm::DicomReader* reader )
{

  if ( reader )
  {

    std::string manufacturer = reader->getManufacturerName();
    std::string storageUID = reader->getStorageUID();
    std::map< std::string, std::map< std::string,
                                     dcm::DicomReader* > >::iterator
      m = _readers.find( manufacturer );

    if ( m == _readers.end() )
    {

#ifdef MINI_VIEWER
      std::cout << "Register reader : " << manufacturer
                << " - " << storageUID << std::endl;
#endif

      std::map< std::string, dcm::DicomReader* > readers;

      readers.insert( std::make_pair( storageUID, reader ) );
      _readers.insert( std::make_pair( manufacturer, readers ) );

      return true;

    }
    else
    {

      std::map< std::string, dcm::DicomReader* >::const_iterator
        r = m->second.find( storageUID );

      if ( r == m->second.end() )
      {

#ifdef MINI_VIEWER
        std::cout << "Register reader : " << manufacturer
                  << " - " << storageUID << std::endl;
#endif

        m->second.insert( std::make_pair( storageUID, reader ) );

        return true;

      }

    }

  }

  return false;

}


bool dcm::DicomReaderFactory::check( const dcm::DatasetModule& datasetModule,
                                     dcm::DirectoryParser& directory,
                                     dcm::DataInfo& dataInfo,
                                     dcm::DicomDatasetHeader& datasetHeader )
{

  dcm::DicomReader* reader = getReader( datasetModule );

  if ( reader )
  {

    return reader->check( datasetModule, directory, dataInfo, datasetHeader );

  }

  std::cout << "Reader for this dataset not supported yet" << std::endl;

  return false;

}


bool dcm::DicomReaderFactory::getHeader( 
                                        const dcm::DatasetModule& datasetModule,
                                        dcm::HeaderProxy& header,
                                        dcm::DataInfo& dataInfo,
                                        dcm::DicomDatasetHeader& datasetHeader )
{

  dcm::DicomReader* reader = getReader( datasetModule );

  if ( reader )
  {

    return reader->getHeader( header, dataInfo, datasetHeader );

  }

  return false;

}


bool dcm::DicomReaderFactory::read( const dcm::DatasetModule& datasetModule,
                                    dcm::DicomDatasetHeader& datasetHeader,
                                    dcm::DicomProxy& proxy )
{

  dcm::DicomReader* reader = getReader( datasetModule );

  if ( reader )
  {

    return reader->read( datasetHeader, proxy );

  }

  return false;

}


dcm::DicomReader* dcm::DicomReaderFactory::getReader(
                                       const dcm::DatasetModule& datasetModule )
{

  std::map< std::string, std::map< std::string,
                                   dcm::DicomReader* > >::const_iterator
    m = _readers.find( datasetModule.getManufacturer() );

  if ( m == _readers.end() )
  {

    m = _readers.find( "Generic" );

  }

  if ( m != _readers.end() )
  {

    std::map< std::string, dcm::DicomReader* >::const_iterator
      r = m->second.find( datasetModule.getSOPClassUID() );

    if ( ( r == m->second.end() ) && 
         ( datasetModule.getManufacturer() != "Generic" ) )
    {

      m = _readers.find( "Generic" );

      if ( m != _readers.end() )
      {

        r = m->second.find( datasetModule.getSOPClassUID() );

      }

    }

    if ( r != m->second.end() )
    {

      return r->second;

    }

  }

  return 0;

}
