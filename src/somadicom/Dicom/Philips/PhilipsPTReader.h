#ifndef _PhilipsPTReader_h_
#define _PhilipsPTReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/PTImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>


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

