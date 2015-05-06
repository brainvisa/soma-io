#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericEnhancedMRReader.h>
#include <soma-io/Dicom/DicomIO.h>
#else
#include <Dicom/GenericEnhancedMRReader.h>
#include <Dicom/DicomIO.h>
#endif

dcm::GenericEnhancedMRReader::GenericEnhancedMRReader()
                            : dcm::EnhancedMRImageStorageReader(),
                              dcm::Singleton< dcm::GenericEnhancedMRReader >()
{
}


RegisterDicomReaderFunction( GenericEnhancedMRReader );
