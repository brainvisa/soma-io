#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericUSReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericUSReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


soma::GenericUSReader::GenericUSReader()
                     : soma::USImageStorageReader(),
                       soma::Singleton< soma::GenericUSReader >()
{
}


RegisterDicomReaderFunction( GenericUSReader );
