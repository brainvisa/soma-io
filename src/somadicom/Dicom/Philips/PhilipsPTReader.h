#ifndef _PhilipsPTReader_h_
#define _PhilipsPTReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/PTImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/PTImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif


class DcmDataset;


namespace soma
{


class PhilipsPTReader : public PTImageStorageReader,
                        public Singleton< PhilipsPTReader >
{

  public:

    std::string getManufacturerName();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& info,
                            DicomDatasetHeader& datasetHeader );

  protected:

    friend class Singleton< PhilipsPTReader >;

    PhilipsPTReader();
    ~PhilipsPTReader();

};


}


#endif

