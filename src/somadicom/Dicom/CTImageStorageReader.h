#ifndef _CTImageStorageReader_h_
#define _CTImageStorageReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiFileReader.h>


namespace soma
{


class CTImageStorageReader : public MultiFileReader
{

  public:

    std::string getStorageUID();

    virtual bool getHeader( Header& header, DataInfo& info );

  protected:

    CTImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomProxy& proxy, Callback* progress = 0 );

};


}


#endif
