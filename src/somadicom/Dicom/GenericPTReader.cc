#include <soma-io/Dicom/GenericPTReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>


soma::GenericPTReader::GenericPTReader()
                     : soma::PTImageStorageReader(),
                       soma::Singleton< soma::GenericPTReader >()
{
}


RegisterDicomReaderFunction( GenericPTReader );
