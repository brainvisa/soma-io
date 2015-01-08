#ifndef _EnhancedMRImageStorageRerader_h_
#define _EnhancedMRImageStorageRerader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiSliceReader.h>

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
    virtual bool readData( DicomProxy& proxy, Callback* progress = 0 );

    virtual bool buildIndexLut( DcmDataset* dataset );

    std::vector< int32_t > m_indexLut;

};


}


#endif
