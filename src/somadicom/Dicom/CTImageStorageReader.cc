#include <soma-io/Dicom/CTImageStorageReader.h>
#include <soma-io/Container/Data.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/DicomDataContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::CTImageStorageReader::CTImageStorageReader()
                          : soma::MultiFileReader()
{
}


std::string soma::CTImageStorageReader::getStorageUID()
{

  return UID_CTImageStorage;

}


bool soma::CTImageStorageReader::readHeader( DcmDataset* dataset )
{

  m_dataInfo.m_slices = int32_t( m_slices.size() );

  return soma::MultiFileReader::readHeader( dataset );

}


bool soma::CTImageStorageReader::readData( soma::Data& data,
                                           soma::Callback* progress )
{

  soma::DataInfo dataInfo( m_dataInfo );

  if ( data.Create( dataInfo ) )
  {

    soma::DicomDataContext context( m_slices, 
                                    data, 
                                    false,
                                    progress );
    carto::ThreadedLoop threadedLoop( &context,
                                      0,
                                      int32_t( m_slices.size() ) );

    threadedLoop.launch();

    return true;

  }

  return false;

}
