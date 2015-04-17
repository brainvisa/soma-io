#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericUSMultiframeReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericUSMultiframeReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


dcm::GenericUSMultiframeReader::GenericUSMultiframeReader()
                              : dcm::USMultiframeImageStorageReader(),
                                dcm::Singleton< 
                                              dcm::GenericUSMultiframeReader >()
{
}


RegisterDicomReaderFunction( GenericUSMultiframeReader );
