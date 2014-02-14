#ifndef _DataInfo_h_
#define _DataInfo_h_


#include <soma-io/Container/Vector.h>
#include <soma-io/Container/DatasetHeader.h>
#include <soma-io/Transformation/PatientOrientation.h>
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

    double m_pixelSpacingX;
    double m_pixelSpacingY;
    double m_pixelSpacingZ;
    double m_spacingBetweenSlices;
    double m_repetitionTime;

    bool m_modalityLUTApplied;
    std::vector< double > m_slope;
    std::vector< double > m_intercept;

    int32_t m_fileCount;
    std::vector< DatasetHeader > m_datasetHeader;

    Vector m_rowVec;
    Vector m_colVec;
    Vector m_normVec;

    PatientOrientation m_patientOrientation;

};


}


#endif
