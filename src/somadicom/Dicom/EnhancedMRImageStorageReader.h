#ifndef _EnhancedMRImageStorageRerader_h_
#define _EnhancedMRImageStorageRerader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiSliceReader.h>
#else
#include <Dicom/MultiSliceReader.h>
#endif

#include <string>


namespace soma
{


class EnhancedMRImageStorageReader : public MultiSliceReader
{

  public:

    std::string getStorageUID();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& info,
                            DicomDatasetHeader& datasetHeader );

  protected:

    EnhancedMRImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );

    virtual bool buildIndexLut( DcmDataset* dataset );

};


}


#endif
