#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericMRReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericMRReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


dcm::GenericMRReader::GenericMRReader()
                    : dcm::MRImageStorageReader(),
                      dcm::Singleton< dcm::GenericMRReader >()
{
}


RegisterDicomReaderFunction( GenericMRReader );
