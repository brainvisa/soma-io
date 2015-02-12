#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericCRReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericCRReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


soma::GenericCRReader::GenericCRReader()
                     : soma::CRImageStorageReader(),
                       soma::Singleton< soma::GenericCRReader >()
{
}


RegisterDicomReaderFunction( GenericCRReader );
