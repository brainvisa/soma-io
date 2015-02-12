#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericUSMultiframeReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericUSMultiframeReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


soma::GenericUSMultiframeReader::GenericUSMultiframeReader()
                               : soma::USMultiframeImageStorageReader(),
                                 soma::Singleton< 
                                             soma::GenericUSMultiframeReader >()
{
}


RegisterDicomReaderFunction( GenericUSMultiframeReader );
