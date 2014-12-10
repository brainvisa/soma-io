#include <soma-io/Dicom/MRImageStorageReader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/DicomDataContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::MRImageStorageReader::MRImageStorageReader()
                          : soma::MultiFileReader()
{
}


std::string soma::MRImageStorageReader::getStorageUID()
{

  return UID_MRImageStorage;

}


bool soma::MRImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    Float64 tmpFloat;

    if ( dataset->findAndGetFloat64( DCM_RepetitionTime, tmpFloat ).good() )
    {

      m_dataInfo->m_repetitionTime= (double)tmpFloat / 1000.0;

    }

    return soma::MultiFileReader::readHeader( dataset );

  }

  return false;

}


bool soma::MRImageStorageReader::readData( soma::DicomProxy& proxy,
                                           soma::Callback* progress )
{

  if ( proxy.allocate() )
  {

    soma::DicomDataContext context( m_slices, proxy, true, progress );
    carto::ThreadedLoop threadedLoop( &context,
                                     0,
                                     int32_t( m_slices.size() ) );

    threadedLoop.launch();

    return true;

  }

  return false;

}
