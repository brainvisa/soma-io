#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GenericUSReader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#else
#include <Dicom/GenericUSReader.h>
#include <Dicom/DicomReaderFactory.h>
#endif


dcm::GenericUSReader::GenericUSReader()
                    : dcm::USImageStorageReader(),
                      dcm::Singleton< dcm::GenericUSReader >()
{
}


RegisterDicomReaderFunction( GenericUSReader );
