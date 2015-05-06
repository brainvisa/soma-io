#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericCTReader.h>
#include <soma-io/Dicom/DicomIO.h>
#else
#include <Dicom/GenericCTReader.h>
#include <Dicom/DicomIO.h>
#endif


dcm::GenericCTReader::GenericCTReader()
                    : dcm::CTImageStorageReader(),
                      dcm::Singleton< dcm::GenericCTReader >()
{
}


RegisterDicomReaderFunction( GenericCTReader );
