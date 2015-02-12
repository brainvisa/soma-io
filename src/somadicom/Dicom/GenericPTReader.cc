#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericPTReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericPTReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


soma::GenericPTReader::GenericPTReader()
                     : soma::PTImageStorageReader(),
                       soma::Singleton< soma::GenericPTReader >()
{
}


RegisterDicomReaderFunction( GenericPTReader );
