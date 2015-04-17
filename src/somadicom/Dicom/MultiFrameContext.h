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


namespace dcm
{


class MultiFrameDicomImage;


#ifdef SOMA_IO_DICOM
class MultiFrameContext : public carto::LoopContext
#else
class MultiFrameContext : public LoopContext
#endif
{

  public:

    MultiFrameContext( MultiFrameDicomImage& dicomImage );

    void doIt( int32_t startIndex, int32_t count );

  private:

    MultiFrameDicomImage& _dicomImage;

};


}


#endif
