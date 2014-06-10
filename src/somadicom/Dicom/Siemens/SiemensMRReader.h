#ifndef _SiemensMRReader_h_
#define _SiemensMRReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MRImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>


class DcmDataset;


namespace soma
{


class Demosaicer;


class SiemensMRReader : public MRImageStorageReader,
                        public Singleton< SiemensMRReader >
{

  public:

    std::string getManufacturerName();

  protected:

    friend class Singleton< SiemensMRReader >;

    SiemensMRReader();
    ~SiemensMRReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( Data& data, Callback* progress = 0 );

  private:

    Demosaicer* m_demosaicer;
    bool m_mosaic;
    int32_t m_sliceCount;

};


}


#endif
