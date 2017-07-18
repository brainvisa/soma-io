#ifndef _EnhancedMRImageStorageReader_h_
#define _EnhancedMRImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/EnhancedReader.h>
#else
#include <Dicom/EnhancedReader.h>
#endif


namespace dcm
{


class EnhancedMRImageStorageReader : public EnhancedReader
{

  public:

    std::string getStorageUID();

  protected:

    EnhancedMRImageStorageReader();

};


}


#endif
