#include <soma-io/Dicom/PTImageStorageReader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/DicomDataContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::PTImageStorageReader::PTImageStorageReader()
                          : soma::MultiFileReader()
{
}


std::string soma::PTImageStorageReader::getStorageUID()
{

#if OFFIS_DCMTK_VERSION_NUMBER >= 360

  return UID_PositronEmissionTomographyImageStorage;

#else

  return UID_PETImageStorage;

#endif

}


bool soma::PTImageStorageReader::readData( soma::DicomProxy& proxy,
                                           soma::Callback* progress )
{

  if ( proxy.allocate() )
  {

    soma::DicomDataContext context( m_slices, proxy, false, progress );
    carto::ThreadedLoop threadedLoop( &context,
                                      0,
                                      int32_t( m_slices.size() ) );

    threadedLoop.launch();

    return true;

  }

  return false;

}
