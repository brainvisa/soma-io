#ifndef _MultiFrameContext_h_
#define _MultiFrameContext_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Thread/LoopContext.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>


namespace soma
{


class DicomProxy;


class MultiFrameContext : public LoopContext
{

  public:

    MultiFrameContext( DicomImage& dcmImage, 
                       DicomProxy& proxy,
                       bool fillSlices = false );

    void doIt( int32_t startIndex, int32_t count );

  private:

    DicomImage& _dcmImage;
    DicomProxy& _proxy;
    bool _fillSlices;

};


}


#endif
