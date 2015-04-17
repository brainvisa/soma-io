#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericNMReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericNMReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


dcm::GenericNMReader::GenericNMReader()
                    : dcm::NMImageStorageReader(),
                      dcm::Singleton< dcm::GenericNMReader >()
{
}


RegisterDicomReaderFunction( GenericNMReader );
