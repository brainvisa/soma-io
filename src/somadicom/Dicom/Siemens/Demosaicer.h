#ifndef _Demosaicer_h_
#define _Demosaicer_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Container/Vector.h>
#include <soma-io/Utils/StdInt.h>
#include <soma-io/Dicom/soma_osconfig.h>

#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>


namespace soma
{


class Data;


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
                   Data& data,
                   int32_t slice,
                   int32_t t );

    Vector demosaic( const Vector& pos );

  private:

    Vector m_rowVector;
    Vector m_columnVector;
    int32_t m_mosaicSizeX;
    int32_t m_mosaicSizeY;
    int32_t m_sizeX;
    int32_t m_sizeY;
    int32_t m_sizeZ;
    int32_t m_divider;
    double m_resolutionX;
    double m_resolutionY;
    double m_resolutionZ;

};


}


#endif
