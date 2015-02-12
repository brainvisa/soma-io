#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericNMReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericNMReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


soma::GenericNMReader::GenericNMReader()
                     : soma::NMImageStorageReader(),
                       soma::Singleton< soma::GenericNMReader >()
{
}


RegisterDicomReaderFunction( GenericNMReader );
