#ifndef _EnhancedIndexModule_h_
#define _EnhancedIndexModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/DicomModule.h>
#include <Utils/StdInt.h>
#endif

#include <vector>


namespace soma
{


class EnhancedIndexModule : public DicomModule
{

  public:

    EnhancedIndexModule();

    virtual bool parseDataset( DcmDataset* dataset );
    bool parseHeader( DicomDatasetHeader& datasetHeader );

    int32_t getNumberOfFrames() const;
    int32_t getZIndex() const;
    int32_t getTIndex() const;
    int32_t getZCount() const;
    const std::vector< int32_t >& getIndices() const;

  protected:

    int32_t _numberOfFrames;
    int32_t _zIndex;
    int32_t _tIndex;
    int32_t _zCount;
    std::vector< int32_t > _indices;

};


}


#endif

