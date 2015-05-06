#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericCRReader.h>
#include <soma-io/Dicom/DicomIO.h>
#else
#include <Dicom/GenericCRReader.h>
#include <Dicom/DicomIO.h>
#endif


dcm::GenericCRReader::GenericCRReader()
                    : dcm::CRImageStorageReader(),
                      dcm::Singleton< dcm::GenericCRReader >()
{
}


RegisterDicomReaderFunction( GenericCRReader );
