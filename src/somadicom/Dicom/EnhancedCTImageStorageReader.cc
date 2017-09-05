#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/EnhancedCTImageStorageReader.h>
#else
#include <Dicom/EnhancedCTImageStorageReader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::EnhancedCTImageStorageReader::EnhancedCTImageStorageReader()
                                 : dcm::EnhancedReader()
{
}


std::string dcm::EnhancedCTImageStorageReader::getStorageUID()
{

  return UID_EnhancedCTImageStorage;

}
