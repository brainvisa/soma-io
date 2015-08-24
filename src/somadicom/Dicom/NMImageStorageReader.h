#ifndef _NMImageStorageReader_h_
#define _NMImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiSliceReader.h>
#else
#include <Dicom/MultiSliceReader.h>
#endif


class DcmDataset;


namespace dcm
{


class NMImageStorageReader : public MultiSliceReader
{

  public:

    std::string getStorageUID();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& info,
                            DicomDatasetHeader& datasetHeader );

  protected:

    NMImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool buildIndexLut(DcmDataset*);

};


}


#endif
