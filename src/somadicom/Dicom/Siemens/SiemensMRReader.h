#ifndef _SiemensMRReader_h_
#define _SiemensMRReader_h_


#include <Dicom/MRImageStorageReader.h>
#include <Pattern/Singleton.h>


class DcmDataset;


namespace soma
{


class SiemensMRReader : public MRImageStorageReader,
                        public Singleton< SiemensMRReader >
{

  public:

    std::string getManufacturerName();

  protected:

    friend class Singleton< SiemensMRReader >;

    SiemensMRReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( Data& data, Callback* progress = 0 );

  private:

    bool m_mosaic;
    int32_t m_sliceCount;

};


}


#endif
