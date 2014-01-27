#include <Dicom/GenericNMReader.h>
#include <Dicom/DicomReaderFactory.h>


soma::GenericNMReader::GenericNMReader()
                     : soma::NMImageStorageReader(),
                       soma::Singleton< soma::GenericNMReader >()
{
}


RegisterDicomReaderFunction( GenericNMReader );
