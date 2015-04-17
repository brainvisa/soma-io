#ifndef _MosaicPointerImage_h_
#define _MosaicPointerImage_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Siemens/MosaicDicomImage.h>
#else
#include <Dicom/Siemens/MosaicDicomImage.h>
#endif


namespace dcm
{


class MosaicPointerImage : public MosaicDicomImage
{

  public:

    MosaicPointerImage( DicomProxy& proxy, 
                        ImagePixel::Parameters& parameters,
                        int32_t mosaicSizeX,
                        void* ptr );

};


}


#endif
