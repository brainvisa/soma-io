#ifndef _CTImageStorageReader_h_
#define _CTImageStorageReader_h_


#include <Dicom/MultiFileReader.h>


namespace soma
{


class CTImageStorageReader : public MultiFileReader
{

  public:

    std::string getStorageUID();

  protected:

    CTImageStorageReader();

    virtual bool readData( Data& data, Callback* progress = 0 );

};


}


#endif
