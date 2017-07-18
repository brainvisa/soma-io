#ifndef _USMultiframeImageStorageReader_h_
#define _USMultiframeImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiframeReader.h>
#include <soma-io/Dicom/USReader.h>
#else
#include <Dicom/MultiframeReader.h>
#include <Dicom/USReader.h>
#endif


namespace dcm
{


class USMultiframeImageStorageReader : public MultiframeReader,
                                       public USReader
{

  public:

    std::string getStorageUID();

  protected:

    USMultiframeImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );

};


}


#endif
