#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Dicom/DicomReader.h>
#include <soma-io/System/Directory.h>
#include <soma-io/Pattern/Callback.h>

#include <iostream>


soma::DicomReaderFactory::DicomReaderFactory()
                        : soma::Singleton< soma::DicomReaderFactory >()
{
}


bool soma::DicomReaderFactory::registerReader( soma::DicomReader* reader )
{

  if ( reader )
  {

    std::string manufacturer = reader->getManufacturerName();
    std::string storageUID = reader->getStorageUID();
    std::map< std::string, std::map< std::string,
                                     soma::DicomReader* > >::iterator
      m = m_readers.find( manufacturer );

    if ( m == m_readers.end() )
    {

      std::map< std::string, soma::DicomReader* > readers;

      readers.insert( std::make_pair( storageUID, reader ) );
      m_readers.insert( std::make_pair( manufacturer, readers ) );

      return true;

    }
    else
    {

      std::map< std::string, soma::DicomReader* >::const_iterator
        r = m->second.find( storageUID );

      if ( r == m->second.end() )
      {

        m->second.insert( std::make_pair( storageUID, reader ) );

        return true;

      }

    }

  }

  return false;

}


bool soma::DicomReaderFactory::check( std::string manufacturer,
                                      std::string storageUID,
                                      soma::DirectoryParser& directory,
                                      std::vector< std::string >& fileList,
                                      soma::DataInfo& dataInfo )
{

  soma::DicomReader* reader = getReader( manufacturer, storageUID );

  if ( reader )
  {

    fileList = reader->check( directory, dataInfo );

    return true;

  }

  std::cout << "Reader for this dataset not supported yet" << std::endl;

  return false;

}


bool soma::DicomReaderFactory::read( 
                                    std::string manufacturer,
                                    std::string storageUID,
                                    const std::vector< std::string >& fileList,
                                    soma::Data& data,
                                    soma::Callback* progress )
{

  soma::DicomReader* reader = getReader( manufacturer, storageUID );

  if ( reader )
  {

    return reader->read( fileList, data, progress );

  }

  std::cout << "Reader for this dataset not supported yet" << std::endl;

  return false;

}


soma::DicomReader* soma::DicomReaderFactory::getReader(
                                                      std::string manufacturer,
                                                      std::string storageUID )
{

  std::map< std::string, std::map< std::string,
                                   soma::DicomReader* > >::const_iterator
    m = m_readers.find( manufacturer );

  if ( m == m_readers.end() )
  {

    m = m_readers.find( "Generic" );

  }

  if ( m != m_readers.end() )
  {

    std::map< std::string, soma::DicomReader* >::const_iterator
      r = m->second.find( storageUID );

    if ( ( r == m->second.end() ) && ( manufacturer != "Generic" ) )
    {

      m = m_readers.find( "Generic" );

      if ( m != m_readers.end() )
      {

        r = m->second.find( storageUID );

      }

    }

    if ( r != m->second.end() )
    {

      return r->second;

    }

  }

  return 0;

}
