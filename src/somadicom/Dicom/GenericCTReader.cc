#include <soma-io/Dicom/GenericCTReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>


soma::GenericCTReader::GenericCTReader()
                     : soma::CTImageStorageReader(),
                       soma::Singleton< soma::GenericCTReader >()
{
}


RegisterDicomReaderFunction( GenericCTReader );
