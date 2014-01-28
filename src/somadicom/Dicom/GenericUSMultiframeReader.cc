#include <soma-io/Dicom/GenericUSMultiframeReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>


soma::GenericUSMultiframeReader::GenericUSMultiframeReader()
                               : soma::USMultiframeImageStorageReader(),
                                 soma::Singleton< 
                                             soma::GenericUSMultiframeReader >()
{
}


RegisterDicomReaderFunction( GenericUSMultiframeReader );
