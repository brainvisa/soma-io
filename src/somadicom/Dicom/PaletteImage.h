#ifndef _PaletteImage_h_
#define _PaletteImage_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/ImagePixel.h>
#include <soma-io/Dicom/PaletteModule.h>
#else
#include <Dicom/ImagePixel.h>
#include <Dicom/PaletteModule.h>
#endif


class DcmDataset;


namespace dcm
{


class PaletteImage : public ImagePixel
{

  public:

    PaletteImage( DicomProxy& proxy );

    bool initialize( DcmDataset* dataset );

    void fill( Parameters& parameters,
               OffsetParameters& offsetParameters,
               void* imagePtr, 
               int32_t z, 
               int32_t t,
               int32_t inputSlice = 0 );

  private:

    PaletteModule _paletteModule;

};


}


#endif
