#ifndef _Demosaicer_h_
#define _Demosaicer_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Container/Vector3d.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Container/Vector3d.h>
#include <Utils/StdInt.h>
#endif


namespace dcm
{


class MosaicDicomImage;
class DataInfo;


class Demosaicer
{

  public:

    Demosaicer( const Vector3d< double >& rowCosine,
                const Vector3d< double >& columnCosine,
                int32_t sizeX,
                int32_t sizeY,
                int32_t sizeZ,
                int32_t sliceCount,
                double resolutionX,
                double resolutionY );
    virtual ~Demosaicer();

    int32_t getMosaicSize();
    int32_t getMosaicSizeX();
    void getSize( int32_t& sizeX, int32_t& sizeY, int32_t& sizeZ );

    void demosaic( MosaicDicomImage& dicomImage,
                   DataInfo& data,
                   int32_t slice,
                   int32_t t );

    Vector3d< double > demosaic( const Vector3d< double >& pos );

  private:

    Vector3d< double > _rowVector;
    Vector3d< double > _columnVector;
    int32_t _mosaicSizeX;
    int32_t _mosaicSizeY;
    int32_t _sizeX;
    int32_t _sizeY;
    int32_t _sizeZ;
    int32_t _divider;
    double _resolutionX;
    double _resolutionY;

};


}


#endif
