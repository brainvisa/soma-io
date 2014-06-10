#ifndef _PTImageStorageReader_h_
#define _PTImageStorageReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiFileReader.h>


class DcmDataset;


namespace soma
{


class PTImageStorageReader : public MultiFileReader
{

  public:

    std::string getStorageUID();

  protected:

    PTImageStorageReader();

    virtual bool readData( Data& data, Callback* progress = 0 );

};


}


#endif
