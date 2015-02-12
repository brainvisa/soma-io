#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericMRReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericMRReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


soma::GenericMRReader::GenericMRReader()
                     : soma::MRImageStorageReader(),
                       soma::Singleton< soma::GenericMRReader >()
{
}


RegisterDicomReaderFunction( GenericMRReader );
