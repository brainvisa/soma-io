#ifndef _ImageModule_h_
#define _ImageModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/DicomModule.h>
#include <Utils/StdInt.h>
#endif

#include <string>


namespace soma
{


class ImageModule : public DicomModule
{

  public:

    ImageModule();

    bool parseDataset( DcmDataset* dataset );

    const std::string& getPhotometricInterpretation() const;
    bool isMonochrome() const;
    int32_t getRows() const;
    int32_t getColumns() const;
    double getPixelSpacingX() const;
    double getPixelSpacingY() const;
    int32_t getBitsAllocated() const;
    int32_t getBitsStored() const;
    int32_t getPixelPaddingValue() const;

  private:

    std::string _photometricInterpretation;
    int32_t _rows;
    int32_t _columns;
    double _pixelSpacingX;
    double _pixelSpacingY;
    int32_t _bitsAllocated;
    int32_t _bitsStored;
    int32_t _pixelPaddingValue;

};


}


#endif

