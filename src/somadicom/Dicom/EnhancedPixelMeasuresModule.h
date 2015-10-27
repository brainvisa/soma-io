#ifndef _EnhancedPixelMeasuresModule_h_
#define _EnhancedPixelMeasuresModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#else
#include <Dicom/DicomModule.h>
#endif


class DcmDataset;


namespace dcm
{


class EnhancedPixelMeasuresModule : public DicomModule
{

  public:

    EnhancedPixelMeasuresModule();

    bool parseItem( DcmItem* item );
    bool parseDataset( DcmDataset* dataset );

    double getSliceThickness() const;
    double getPixelSpacingX() const;
    double getPixelSpacingY() const;

  private:

    double _sliceThickness;
    double _pixelSpacingX;
    double _pixelSpacingY;

};


}


#endif
