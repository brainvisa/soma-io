#include <soma-io/Dicom/GenericEnhancedMRReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>


soma::GenericEnhancedMRReader::GenericEnhancedMRReader()
                             : soma::EnhancedMRImageStorageReader(),
                               soma::Singleton< 
                                               soma::GenericEnhancedMRReader >()
{
}


RegisterDicomReaderFunction( GenericEnhancedMRReader );
