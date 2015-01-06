#ifndef _MRImageStorageReader_h_
#define _MRImageStorageReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiFileReader.h>


class DcmDataset;


namespace soma
{


class MRImageStorageReader : public MultiFileReader
{

  public:

    std::string getStorageUID();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& info,
                            DicomDatasetHeader& datasetHeader );

  protected:

    MRImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomProxy& proxy, Callback* progress = 0 );

};


}


#endif
