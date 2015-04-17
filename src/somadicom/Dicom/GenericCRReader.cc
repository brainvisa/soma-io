#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericCRReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericCRReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


dcm::GenericCRReader::GenericCRReader()
                    : dcm::CRImageStorageReader(),
                      dcm::Singleton< dcm::GenericCRReader >()
{
}


RegisterDicomReaderFunction( GenericCRReader );
