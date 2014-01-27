#ifndef _USImageStorageReader_h_
#define _USImageStorageReader_h_


#include <Dicom/USReader.h>

#include <string>


class DcmDataset;


namespace soma
{


class Data;
class Callback;


class USImageStorageReader : public USReader
{

  public:

    std::string getStorageUID();

  protected:

    USImageStorageReader();

    virtual bool readData( Data& data, Callback* progress = 0 );

};


}


#endif
