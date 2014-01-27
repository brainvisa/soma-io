#include <Dicom/GenericUSReader.h>
#include <Dicom/DicomReaderFactory.h>


soma::GenericUSReader::GenericUSReader()
                     : soma::USImageStorageReader(),
                       soma::Singleton< soma::GenericUSReader >()
{
}


RegisterDicomReaderFunction( GenericUSReader );
