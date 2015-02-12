#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericCTReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericCTReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


soma::GenericCTReader::GenericCTReader()
                     : soma::CTImageStorageReader(),
                       soma::Singleton< soma::GenericCTReader >()
{
}


RegisterDicomReaderFunction( GenericCTReader );
