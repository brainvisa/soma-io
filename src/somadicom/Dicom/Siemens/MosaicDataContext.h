#ifndef _MosaicDataContext_h_
#define _MosaicDataContext_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Thread/LoopContext.h>
#include <Utils/StdInt.h>
#endif

#include <string>
#include <vector>


namespace soma
{


class DicomDatasetHeader;
class DicomProxy;
class Demosaicer;


class MosaicDataContext : public LoopContext
{

  public:

    MosaicDataContext( DicomDatasetHeader& datasetHeader,
                       DicomProxy& proxy,
                       Demosaicer& demosaicer );

    void doIt( int32_t startIndex, int32_t count );

  private:

    DicomDatasetHeader& _datasetHeader;
    DicomProxy& _proxy;
    Demosaicer& _demosaicer;

};


}


#endif
