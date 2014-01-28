#ifndef _NMImageStorageReader_h_
#define _NMImageStorageReader_h_


#include <soma-io/Dicom/DicomReader.h>


class DcmDataset;


namespace soma
{


class NMImageStorageReader : public DicomReader
{

  public:

    std::string getStorageUID();

  protected:

    NMImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( Data& data, Callback* progress = 0 );

};


}


#endif
