#include <soma-io/Dicom/PTImageStorageReader.h>
#include <soma-io/Container/Data.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/DicomDataContext.h>
#include <Thread/ThreadedLoop.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <sstream>


soma::PTImageStorageReader::PTImageStorageReader()
                          : soma::MultiFileReader()
{
}


std::string soma::PTImageStorageReader::getStorageUID()
{

  return UID_PositronEmissionTomographyImageStorage;

}


bool soma::PTImageStorageReader::readData( soma::Data& data,
                                           soma::Callback* progress )
{

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
