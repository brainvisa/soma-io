#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericEnhancedCTReader.h>
#include <soma-io/Dicom/DicomIO.h>
#else
#include <Dicom/GenericEnhancedCTReader.h>
#include <Dicom/DicomIO.h>
#endif


dcm::GenericEnhancedCTReader::GenericEnhancedCTReader()
                            : dcm::EnhancedCTImageStorageReader(),
                              dcm::Singleton< dcm::GenericEnhancedCTReader >()
{
}


RegisterDicomReaderFunction( GenericEnhancedCTReader );
