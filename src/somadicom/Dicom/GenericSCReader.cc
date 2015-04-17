#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericSCReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericSCReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


dcm::GenericSCReader::GenericSCReader()
                    : dcm::SCImageStorageReader(),
                      dcm::Singleton< dcm::GenericSCReader >()
{
}


RegisterDicomReaderFunction( GenericSCReader );
