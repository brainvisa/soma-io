#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericMRReader.h>
#include <soma-io/Dicom/DicomIO.h>
#else
#include <Dicom/GenericMRReader.h>
#include <Dicom/DicomIO.h>
#endif


dcm::GenericMRReader::GenericMRReader()
                    : dcm::MRImageStorageReader(),
                      dcm::Singleton< dcm::GenericMRReader >()
{
}


RegisterDicomReaderFunction( GenericMRReader );
