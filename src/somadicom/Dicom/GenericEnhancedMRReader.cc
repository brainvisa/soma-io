#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericEnhancedMRReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericEnhancedMRReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif

dcm::GenericEnhancedMRReader::GenericEnhancedMRReader()
                            : dcm::EnhancedMRImageStorageReader(),
                              dcm::Singleton< dcm::GenericEnhancedMRReader >()
{
}


RegisterDicomReaderFunction( GenericEnhancedMRReader );
