#include <Dicom/GenericCTReader.h>
#include <Dicom/DicomReaderFactory.h>


soma::GenericCTReader::GenericCTReader()
                     : soma::CTImageStorageReader(),
                       soma::Singleton< soma::GenericCTReader >()
{
}


RegisterDicomReaderFunction( GenericCTReader );
