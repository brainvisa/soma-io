#ifndef _DicomDataContext_h_
#define _DicomDataContext_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/ImagePixel.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/ImagePixel.h>
#include <Thread/LoopContext.h>
#include <Utils/StdInt.h>
#endif

#include <vector>
#include <string>


namespace soma
{


class DicomDatasetHeader;
class DicomProxy;


class DicomDataContext : public LoopContext
{

  public:

    DicomDataContext( DicomDatasetHeader& datasetHeader,
                      DicomProxy& proxy,
                      const std::vector< int32_t >& selection );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const std::vector< int32_t >& _selection;
    std::vector< std::string >& _fileList;
    std::vector< int32_t >& _lut;
    DicomProxy& _proxy;
    ImagePixel::Parameters _parameters;

};


}


#endif
