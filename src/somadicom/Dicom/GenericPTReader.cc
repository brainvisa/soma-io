#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericPTReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericPTReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


dcm::GenericPTReader::GenericPTReader()
                    : dcm::PTImageStorageReader(),
                      dcm::Singleton< dcm::GenericPTReader >()
{
}


RegisterDicomReaderFunction( GenericPTReader );
