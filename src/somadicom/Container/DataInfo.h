#ifndef _DataInfo_h_
#define _DataInfo_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Transformation/PatientOrientation.h>
#include <soma-io/Object/BinaryHeader.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Transformation/PatientOrientation.h>
#include <Object/BinaryHeader.h>
#include <Utils/StdInt.h>
#endif

#include <vector>


namespace dcm
{


class DataInfo
{

  public:

    DataInfo();
    DataInfo( const DataInfo& other );
    virtual ~DataInfo();

    void initialize();
    void clear();

    int32_t _depth;
    int32_t _spp;
    int32_t _bpp;
    int32_t _bitsStored;
    int32_t _pixelRepresentation;
    bool _invertLut;

    int32_t _minimum;
    int32_t _maximum;

    int32_t _width;
    int32_t _height;
    int32_t _slices;
    int32_t _frames;

    int32_t _sliceSize;
    int32_t _volumeSize;
    int32_t _datasetSize;

    Vector3d< double > _resolution;
    double _spacingBetweenSlices;
    double _repetitionTime;

    int32_t _fileCount;

    Vector3d< double > _rowVec;
    Vector3d< double > _colVec;
    Vector3d< double > _origin;

    PatientOrientation _patientOrientation;
    BinaryHeader _datasetHeader;

    BoundingBox< int32_t > _boundingBox;

    bool _mosaic;
    bool _noFlip;
    bool _noDemosaic;

};


}


#endif
