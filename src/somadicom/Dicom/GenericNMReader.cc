#include <soma-io/Dicom/GenericNMReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>


soma::GenericNMReader::GenericNMReader()
                     : soma::NMImageStorageReader(),
                       soma::Singleton< soma::GenericNMReader >()
{
}


RegisterDicomReaderFunction( GenericNMReader );
