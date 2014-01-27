#include <Dicom/CTImageStorageReader.h>
#include <Container/Data.h>
#include <Pattern/Callback.h>
#include <Dicom/DicomDataContext.h>
#include <Thread/ThreadedLoop.h>
#include <Utils/StdInt.h>

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::CTImageStorageReader::CTImageStorageReader()
                          : soma::MultiFileReader()
{
}


std::string soma::CTImageStorageReader::getStorageUID()
{

  return UID_CTImageStorage;

}


bool soma::CTImageStorageReader::readData( soma::Data& data,
                                           soma::Callback* progress )
{

  m_dataInfo.m_slices = int32_t( m_slices.size() );

  soma::DataInfo dataInfo( m_dataInfo );
  setOrientation( dataInfo );

  if ( data.Create( dataInfo ) )
  {

    soma::DicomDataContext context( m_slices, 
                                    data, 
                                    false,
                                    progress );
    soma::ThreadedLoop threadedLoop( &context,
                                     0,
                                     int32_t( m_slices.size() ) );

    threadedLoop.launch();

    return true;

  }

  return false;

}
