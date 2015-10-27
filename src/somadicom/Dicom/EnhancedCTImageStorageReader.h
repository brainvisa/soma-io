#ifndef _EnhancedCTImageStorageReader_h_
#define _EnhancedCTImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/EnhancedReader.h>
#else
#include <Dicom/EnhancedReader.h>
#endif


namespace dcm
{


class EnhancedCTImageStorageReader : public EnhancedReader
{

  public:

    std::string getStorageUID();

  protected:

    EnhancedCTImageStorageReader();

};


}


#endif
