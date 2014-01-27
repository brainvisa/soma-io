#include <Dicom/GenericMRReader.h>
#include <Dicom/DicomReaderFactory.h>


soma::GenericMRReader::GenericMRReader()
                     : soma::MRImageStorageReader(),
                       soma::Singleton< soma::GenericMRReader >()
{
}


RegisterDicomReaderFunction( GenericMRReader );
