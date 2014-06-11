#include <soma-io/Dicom/PTImageStorageReader.h>
#include <soma-io/Container/Data.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/DicomDataContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>

// Fix for DCMTK 3.5.4
#ifndef UID_PositronEmissionTomographyImageStorage
  #define UID_PositronEmissionTomographyImageStorage "1.2.840.10008.5.1.4.1.1.128"
#endif

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
