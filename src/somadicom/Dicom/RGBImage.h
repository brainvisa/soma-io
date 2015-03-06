#ifndef _RGBImage_h_
#define _RGBImage_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/ImagePixel.h>
#else
#include <Dicom/ImagePixel.h>
#endif


namespace soma
{


class RGBImage : public ImagePixel
{

  public:

    RGBImage( DicomProxy& proxy, int32_t planarConfiguration );

    void fill( Parameters& parameters, 
               OffsetParameters& offsetParameters,
               void* imagePtr, 
               int32_t z, 
               int32_t t,
               int32_t inputSlice = 0 );

  private:

    int32_t _planarConfiguration;

};


}



#endif
