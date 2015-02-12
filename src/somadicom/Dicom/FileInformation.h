#ifndef _FileInformation_h_
#define _FileInformation_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Container/Vector.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Container/Vector.h>
#include <Utils/StdInt.h>
#endif


namespace soma
{


class FileInformation
{

  public:

    FileInformation();

    int32_t _bufferIndex;
    int32_t _instanceNumber;
    Vector _imagePosition;

};


}

#endif
