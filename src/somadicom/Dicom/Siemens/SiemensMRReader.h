#ifndef _SiemensMRReader_h_
#define _SiemensMRReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MRImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>


class DcmDataset;


namespace soma
{


class DirectoryParser;
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
    virtual bool readData( DicomProxy& proxy, Callback* progress = 0 );

    virtual std::vector< std::string > sortFiles( DirectoryParser& directory );

  private:

    Demosaicer* m_demosaicer;
    int32_t m_sliceCount;

};


}


#endif
