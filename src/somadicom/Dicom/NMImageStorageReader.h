#ifndef _NMImageStorageReader_h_
#define _NMImageStorageReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomReader.h>


class DcmDataset;


namespace soma
{


class NMImageStorageReader : public DicomReader
{

  public:

    std::string getStorageUID();

    virtual bool getHeader( Header& header, DataInfo& info );

  protected:

    NMImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomProxy& proxy, Callback* progress = 0 );

};


}


#endif
