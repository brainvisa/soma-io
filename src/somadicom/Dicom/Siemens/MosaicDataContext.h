#ifndef _MosaicDataContext_h_
#define _MosaicDataContext_h_


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

#include <string>
#include <vector>


namespace dcm
{


class DicomDatasetHeader;
class DicomProxy;
class Demosaicer;


#ifdef SOMA_IO_DICOM
class MosaicDataContext : public carto::LoopContext
#else
class MosaicDataContext : public LoopContext
#endif
{

  public:

    MosaicDataContext( DicomDatasetHeader& datasetHeader,
                       DicomProxy& proxy,
                       Demosaicer& demosaicer );

    void doIt( int32_t startIndex, int32_t count );

  private:

    std::vector< std::string >& _fileList;
    std::vector< int32_t >& _lut;
    DicomProxy& _proxy;
    ImagePixel::Parameters _parameters;
    Demosaicer& _demosaicer;

};


}


#endif
