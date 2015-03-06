#ifndef _MosaicDicomImage_h_
#define _MosaicDicomImage_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomImage.h>
#else
#include <Dicom/DicomImage.h>
#endif


namespace soma
{


class MosaicDicomImage : public DicomImage
{

  public:

    MosaicDicomImage( DicomProxy& proxy, 
                      ImagePixel::Parameters& parameters,
                      int32_t mosaicSizeX );

    void setLineShift( int32_t lineShift );
    void setOffset( int32_t offset );

};


}



#endif
