#ifndef _MonochromeImage_h_
#define _MonochromeImage_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/ImagePixel.h>
#else
#include <Dicom/ImagePixel.h>
#endif


namespace soma
{


template < typename T >
class MonochromeImage : public ImagePixel
{

  public:

    MonochromeImage( DicomProxy& proxy );

    void fill( Parameters& parameters, 
               OffsetParameters& offsetParameters,
               void* imagePtr, 
               int32_t z, 
               int32_t t,
               int32_t inputSlice = 0 );

};


}


#endif
