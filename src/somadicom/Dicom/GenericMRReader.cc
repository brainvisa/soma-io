#include <soma-io/Dicom/GenericMRReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>


soma::GenericMRReader::GenericMRReader()
                     : soma::MRImageStorageReader(),
                       soma::Singleton< soma::GenericMRReader >()
{
}


RegisterDicomReaderFunction( GenericMRReader );
