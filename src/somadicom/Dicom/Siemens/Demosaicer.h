#ifndef _Demosaicer_h_
#define _Demosaicer_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Container/Vector.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Container/Vector.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>


namespace soma
{


class DicomProxy;


class Demosaicer
{

  public:

    Demosaicer( const Vector& rowCosine,
                const Vector& columnCosine,
                int32_t sizeX,
                int32_t sizeY,
                int32_t sizeZ,
                int32_t sliceCount,
                double resolutionX,
                double resolutionY,
                double resolutionZ );
    virtual ~Demosaicer();

    int32_t getMosaicSize();
    void getSize( int32_t& sizeX, int32_t& sizeY, int32_t& sizeZ );

    void demosaic( DicomImage& dcmImage,
                   DicomProxy& data,
                   int32_t slice,
                   int32_t t );
    void demosaic( const uint8_t* mosaic, 
                   DicomProxy& data, 
                   int32_t slice,
                   int32_t t );

    Vector demosaic( const Vector& pos );

  private:

    Vector _rowVector;
    Vector _columnVector;
    int32_t _mosaicSizeX;
    int32_t _mosaicSizeY;
    int32_t _sizeX;
    int32_t _sizeY;
    int32_t _sizeZ;
    int32_t _divider;
    double _resolutionX;
    double _resolutionY;
    double _resolutionZ;

};


}


#endif
