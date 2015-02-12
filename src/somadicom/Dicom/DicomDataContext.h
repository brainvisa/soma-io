#ifndef _DicomDataContext_h_
#define _DicomDataContext_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Thread/LoopContext.h>
#include <Utils/StdInt.h>
#endif

#include <vector>


namespace soma
{


class DicomDatasetHeader;
class DicomProxy;


class DicomDataContext : public LoopContext
{

  public:

    DicomDataContext( DicomDatasetHeader& datasetHeader,
                      DicomProxy& proxy,
                      const std::vector< int32_t >& selection,
                      bool applyModalityLUT = true );

    void doIt( int32_t startIndex, int32_t count );

  private:

    DicomDatasetHeader& _datasetHeader;
    DicomProxy& _proxy;
    const std::vector< int32_t >& _selection;

};


}


#endif
