#ifndef _CRImageStorageReader_h_
#define _CRImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/SingleFileReader.h>
#else
#include <Dicom/SingleFileReader.h>
#endif


namespace dcm
{


class CRImageStorageReader : public SingleFileReader
{

  public:

    std::string getStorageUID();

  protected:

    CRImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );

};


}


#endif
