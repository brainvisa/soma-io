#ifndef _PhilipsMRReader_h_
#define _PhilipsMRReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MRImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/MRImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif


class DcmDataset;


namespace soma
{


class PhilipsMRReader : public MRImageStorageReader,
                        public Singleton< PhilipsMRReader >
{

  public:

    std::string getManufacturerName();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& info,
                            DicomDatasetHeader& datasetHeader );

  protected:

    friend class Singleton< PhilipsMRReader >;

    PhilipsMRReader();
    ~PhilipsMRReader();

};


}


#endif

