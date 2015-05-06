#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericUSMultiframeReader.h>
#include <soma-io/Dicom/DicomIO.h>
#else
#include <Dicom/GenericUSMultiframeReader.h>
#include <Dicom/DicomIO.h>
#endif


dcm::GenericUSMultiframeReader::GenericUSMultiframeReader()
                              : dcm::USMultiframeImageStorageReader(),
                                dcm::Singleton< 
                                              dcm::GenericUSMultiframeReader >()
{
}


RegisterDicomReaderFunction( GenericUSMultiframeReader );
