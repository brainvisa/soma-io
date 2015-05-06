#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericNMReader.h>
#include <soma-io/Dicom/DicomIO.h>
#else
#include <Dicom/GenericNMReader.h>
#include <Dicom/DicomIO.h>
#endif


dcm::GenericNMReader::GenericNMReader()
                    : dcm::NMImageStorageReader(),
                      dcm::Singleton< dcm::GenericNMReader >()
{
}


RegisterDicomReaderFunction( GenericNMReader );
