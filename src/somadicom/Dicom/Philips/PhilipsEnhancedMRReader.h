#ifndef _PhilipsEnhancedMRReader_h_
#define _PhilipsEnhancedMRReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/EnhancedMRImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>


class DcmDataset;


namespace soma
{


class PhilipsEnhancedMRReader : public EnhancedMRImageStorageReader,
                                public Singleton< PhilipsEnhancedMRReader >
{

  public:

    std::string getManufacturerName();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& info,
                            DicomDatasetHeader& datasetHeader );

  protected:

    friend class Singleton< PhilipsEnhancedMRReader >;

    PhilipsEnhancedMRReader();
    ~PhilipsEnhancedMRReader();

    virtual bool buildIndexLut( DcmDataset* dataset );

    int32_t m_tIndex;

};


}


#endif

