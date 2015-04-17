#ifndef _SiemensMRReader_h_
#define _SiemensMRReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MRImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/MRImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif


class DcmDataset;


namespace dcm
{


class Demosaicer;


class SiemensMRReader : public MRImageStorageReader,
                        public Singleton< SiemensMRReader >
{

  public:

    std::string getManufacturerName();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& info,
                            DicomDatasetHeader& datasetHeader );

  protected:

    friend class Singleton< SiemensMRReader >;

    SiemensMRReader();
    ~SiemensMRReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomDatasetHeader& datasetHeader,
                           DicomProxy& proxy );

    bool sortFiles( DicomDatasetHeader& datasetHeader );

  private:

    Demosaicer* _demosaicer;
    int32_t _sliceCount;

};


}


#endif
