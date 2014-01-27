#include <Dicom/GenericEnhancedMRReader.h>
#include <Dicom/DicomReaderFactory.h>


soma::GenericEnhancedMRReader::GenericEnhancedMRReader()
                             : soma::EnhancedMRImageStorageReader(),
                               soma::Singleton< 
                                               soma::GenericEnhancedMRReader >()
{
}


RegisterDicomReaderFunction( GenericEnhancedMRReader );
