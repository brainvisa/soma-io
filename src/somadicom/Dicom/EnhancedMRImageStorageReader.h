#ifndef _EnhancedMRImageStorageRerader_h_
#define _EnhancedMRImageStorageRerader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiSliceReader.h>
#else
#include <Dicom/MultiSliceReader.h>
#endif

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
    virtual bool readData( DicomDatasetHeader& datasetHeader,
                           DicomProxy& proxy );

    virtual bool buildIndexLut( DcmDataset* dataset );

    std::vector< int32_t > _indexLut;

};


}


#endif
