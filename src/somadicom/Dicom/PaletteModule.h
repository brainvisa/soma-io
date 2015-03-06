#ifndef _PaletteModule_h_
#define _PaletteModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Dicom/ColorLut.h>
#include <soma-io/Color/Rgba.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/DicomModule.h>
#include <Dicom/ColorLut.h>
#include <Color/Rgba.h>
#include <Utils/StdInt.h>
#endif


namespace soma
{


class PaletteModule : public DicomModule
{

  public:

    PaletteModule();

    bool parseDataset( DcmDataset* dataset );

    Rgba operator()( int32_t index );

  private:

    ColorLut _red;
    ColorLut _green;
    ColorLut _blue;

};


}


#endif
