#ifndef _MultiFrameDicomImage_h_
#define _MultiFrameDicomImage_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomImage.h>
#else
#include <Dicom/DicomImage.h>
#endif


namespace soma
{


class MultiFrameDicomImage : public DicomImage
{

  public:

    MultiFrameDicomImage( DicomProxy& proxy, 
                          ImagePixel::Parameters& parameters );

    void getImagePtr();
    void fillSlab( int32_t start, int32_t count );

    ImagePixel* getImagePixel() { return _image; }
    DcmDataset* getDataset() { return _dataset; }
    DcmPixelData* getPixelData() { return _pixelData; }
    ImagePixel::Parameters* getParam() { return &_parameters; }
    ImagePixel::OffsetParameters* getOffetParam() { return &_offsetParameters; }

};


}


#endif
