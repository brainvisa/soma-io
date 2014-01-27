#ifndef _EnhancedMRImageStorageRerader_h_
#define _EnhancedMRImageStorageRerader_h_


#include <Dicom/MultiSliceReader.h>

#include <string>


class DcmDataset;


namespace soma
{


class EnhancedMRImageStorageReader : public MultiSliceReader
{

  public:

    std::string getStorageUID();

  protected:

    EnhancedMRImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( Data& data, Callback* progress = 0 );

};


}


#endif
