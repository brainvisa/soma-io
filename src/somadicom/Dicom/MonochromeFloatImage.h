#ifndef _dcm_MonochromeFloatImage_h_
#define _dcm_MonochromeFloatImage_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/ImagePixel.h>
#else
#include <Dicom/ImagePixel.h>
#endif


class DcmDataset;


namespace dcm
{


template < typename T >
class MonochromeFloatImage : public ImagePixel
{

  public:

    MonochromeFloatImage( DicomProxy& proxy);

    bool initialize( DcmDataset* dataset );

    void fill( Parameters& parameters, 
               OffsetParameters& offsetParameters,
               void* imagePtr, 
               int32_t z, 
               int32_t t,
               int32_t inputSlice = 0 );

};


}


#endif
