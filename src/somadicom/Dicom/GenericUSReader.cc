#include <soma-io/Dicom/GenericUSReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>


soma::GenericUSReader::GenericUSReader()
                     : soma::USImageStorageReader(),
                       soma::Singleton< soma::GenericUSReader >()
{
}


RegisterDicomReaderFunction( GenericUSReader );
