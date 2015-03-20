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

    bool parseItem( DcmItem* dcmItem );

    int32_t getSamplesPerPixel() const;
    const std::string& getPhotometricInterpretation() const;
    int32_t getPlanarConfiguration() const;
    int32_t getRows() const;
    int32_t getColumns() const;
    double getPixelSpacingX() const;
    double getPixelSpacingY() const;
    int32_t getBitsAllocated() const;
    int32_t getBitsStored() const;
    int32_t getPixelRepresentation() const;
    int32_t getSmallestPixelValue() const;
    int32_t getLargestPixelValue() const;
    int32_t getPixelPaddingValue() const;

  private:

    int32_t _samplesPerPixel;
    std::string _photometricInterpretation;
    int32_t _planarConfiguration;
    int32_t _rows;
    int32_t _columns;
    double _pixelSpacingX;
    double _pixelSpacingY;
    int32_t _bitsAllocated;
    int32_t _bitsStored;
    int32_t _pixelRepresentation;
    int32_t _smallestPixelValue;
    int32_t _largestPixelValue;
    int32_t _pixelPaddingValue;

};


}


#endif

