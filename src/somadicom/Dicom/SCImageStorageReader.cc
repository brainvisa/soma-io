#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/SCImageStorageReader.h>
#else
#include <Dicom/SCImageStorageReader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::SCImageStorageReader::SCImageStorageReader()
                         : dcm::MultiFileReader()
{
}


std::string dcm::SCImageStorageReader::getStorageUID()
{

  return UID_SecondaryCaptureImageStorage;

}
