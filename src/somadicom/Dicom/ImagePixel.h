#ifndef _ImagePixel_h_
#define _ImagePixel_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Utils/StdInt.h>
#endif


class DcmDataset;


namespace dcm
{


class PatientOrientation;
class DicomProxy;


class ImagePixel
{

  public:

    struct Parameters
    {

      Parameters();
      Parameters( DicomProxy& proxy );
      Parameters( const Parameters& other );

      bool noFlip;
      int32_t startX;
      int32_t endX;
      int32_t startY;
      int32_t endY;
      int32_t sizeX;
      int32_t sizeZ;
      int32_t sizeXY;
      int32_t outLowerX;
      int32_t outLowerY;
      int32_t outLowerZ;
      int32_t outLowerT;
      PatientOrientation* transform;

    };

    struct OffsetParameters
    {

      int32_t offset;
      int32_t lineShift;
      int32_t rgbPlaneSize;

    };

    ImagePixel( DicomProxy& proxy );
    virtual ~ImagePixel();

    virtual bool initialize( DcmDataset* dataset );
    virtual void fill( Parameters& parameters, 
                       OffsetParameters& offsetParameters,
                       void* imagePtr, 
                       int32_t z, 
                       int32_t t,
                       int32_t inputSlice = 0 ) = 0;

  protected:

    DicomProxy& _proxy;

};


}


#endif
