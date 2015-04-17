#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/FileInformation.h>
#else
#include <Dicom/FileInformation.h>
#endif


dcm::FileInformation::FileInformation()
                     : _bufferIndex( 0 ),
                       _instanceNumber( 0 )
{
}
