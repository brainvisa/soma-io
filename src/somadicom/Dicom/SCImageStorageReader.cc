#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/SCImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/SCImageStorageReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::SCImageStorageReader::SCImageStorageReader()
                          : soma::MultiFileReader()
{
}


std::string soma::SCImageStorageReader::getStorageUID()
{

  return UID_SecondaryCaptureImageStorage;

}
