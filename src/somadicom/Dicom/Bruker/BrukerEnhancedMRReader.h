#ifndef _BrukerEnhancedMRReader_h_
#define _BrukerEnhancedMRReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/EnhancedMRImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/EnhancedMRImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif


class DcmDataset;


namespace soma
{


class BrukerEnhancedMRReader : public EnhancedMRImageStorageReader,
                               public Singleton< BrukerEnhancedMRReader >
{

  public:

    std::string getManufacturerName();

  protected:

    friend class Singleton< BrukerEnhancedMRReader >;

    BrukerEnhancedMRReader();
    ~BrukerEnhancedMRReader();

    virtual bool buildIndexLut( DcmDataset* dataset );

};


}


#endif

