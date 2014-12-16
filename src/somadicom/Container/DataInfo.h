#ifndef _DataInfo_h_
#define _DataInfo_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Container/Vector.h>
#include <soma-io/Transformation/PatientOrientation.h>
#include <soma-io/Object/BinaryHeader.h>
#include <soma-io/Utils/StdInt.h>

#include <vector>


namespace soma
{


class DataInfo
{

  public:

    DataInfo();
    DataInfo( const DataInfo& other );
    virtual ~DataInfo();

    void initialize();
    void clear();

    int32_t m_depth;
    int32_t m_bpp;
    int32_t m_bitsStored;
    int32_t m_pixelRepresentation;
    bool m_monochrome;

    int32_t m_minimum;
    int32_t m_maximum;

    int32_t m_width;
    int32_t m_height;
    int32_t m_slices;
    int32_t m_frames;

    int32_t m_sliceSize;
    int32_t m_volumeSize;
    int32_t m_datasetSize;

    Vector m_resolution;
    double m_spacingBetweenSlices;
    double m_repetitionTime;

    bool m_modalityLUTApplied;
    std::vector< double > m_slope;
    std::vector< double > m_intercept;

    int32_t m_fileCount;

    Vector m_rowVec;
    Vector m_colVec;
    Vector m_normVec;

    PatientOrientation m_patientOrientation;
    BinaryHeader m_datasetHeader;

    bool m_mosaic;
    bool m_noFlip;
    bool m_noDemosaic;

};


}


#endif
