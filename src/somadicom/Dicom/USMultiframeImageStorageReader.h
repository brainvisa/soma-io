#ifndef _USMultiframeImageStorageReader_h_
#define _USMultiframeImageStorageReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/USReader.h>


class DcmDataset;


namespace soma
{


class Data;
class Callback;


class USMultiframeImageStorageReader : public USReader
{

  public:

    std::string getStorageUID();

  protected:

    USMultiframeImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( Data& data, Callback* progress = 0 );

};


}


#endif
