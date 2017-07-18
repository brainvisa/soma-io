#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/EnhancedMRImageStorageReader.h>
#else
#include <Dicom/EnhancedMRImageStorageReader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::EnhancedMRImageStorageReader::EnhancedMRImageStorageReader()
                                 : dcm::EnhancedReader()
{
}


std::string dcm::EnhancedMRImageStorageReader::getStorageUID()
{

  return UID_EnhancedMRImageStorage;

}
