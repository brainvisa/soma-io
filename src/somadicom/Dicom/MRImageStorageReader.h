#ifndef _MRImageStorageReader_h_
#define _MRImageStorageReader_h_


#include <soma-io/Dicom/MultiFileReader.h>


class DcmDataset;


namespace soma
{


class MRImageStorageReader : public MultiFileReader
{

  public:

    std::string getStorageUID();

  protected:

    MRImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( Data& data, Callback* progress = 0 );

};


}


#endif
