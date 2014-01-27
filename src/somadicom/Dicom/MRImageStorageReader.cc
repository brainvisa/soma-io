#include <Dicom/MRImageStorageReader.h>
#include <Container/Data.h>
#include <Pattern/Callback.h>
#include <Dicom/DicomDataContext.h>
#include <Thread/ThreadedLoop.h>
#include <Utils/StdInt.h>

#include <dcmtk/config/osconfig.h>
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

      m_dataInfo.m_repetitionTime= (double)tmpFloat / 1000.0;

    }

    return soma::MultiFileReader::readHeader( dataset );

  }

  return false;

}


bool soma::MRImageStorageReader::readData( soma::Data& data,
                                           soma::Callback* progress )
{

  soma::DataInfo dataInfo( m_dataInfo );
  setOrientation( dataInfo );

  if ( data.Create( dataInfo ) )
  {

    soma::DicomDataContext context( m_slices, 
                                    data, 
                                    true,
                                    progress );
    soma::ThreadedLoop threadedLoop( &context,
                                     0,
                                     int32_t( m_slices.size() ) );

    threadedLoop.launch();

    return true;

  }

  return false;

}
