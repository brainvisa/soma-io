#include <Dicom/GenericUSMultiframeReader.h>
#include <Dicom/DicomReaderFactory.h>


soma::GenericUSMultiframeReader::GenericUSMultiframeReader()
                               : soma::USMultiframeImageStorageReader(),
                                 soma::Singleton< 
                                             soma::GenericUSMultiframeReader >()
{
}


RegisterDicomReaderFunction( GenericUSMultiframeReader );
