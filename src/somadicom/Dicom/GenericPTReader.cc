#include <Dicom/GenericPTReader.h>
#include <Dicom/DicomReaderFactory.h>


soma::GenericPTReader::GenericPTReader()
                     : soma::PTImageStorageReader(),
                       soma::Singleton< soma::GenericPTReader >()
{
}


RegisterDicomReaderFunction( GenericPTReader );
