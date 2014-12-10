#ifndef _USImageStorageReader_h_
#define _USImageStorageReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/USReader.h>

#include <string>


class DcmDataset;


namespace soma
{


class DicomProxy;
class Callback;


class USImageStorageReader : public USReader
{

  public:

    std::string getStorageUID();

  protected:

    USImageStorageReader();

    virtual bool readData( DicomProxy& proxy, Callback* progress = 0 );

};


}


#endif
