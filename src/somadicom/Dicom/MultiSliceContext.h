#ifndef _MultiSliceContext_h_
#define _MultiSliceContext_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Thread/LoopContext.h>
#include <Utils/StdInt.h>
#endif

#include <vector>


namespace dcm
{


class MultiSliceDicomImage;


#ifdef SOMA_IO_DICOM
class MultiSliceContext : public carto::LoopContext
#else
class MultiSliceContext : public LoopContext
#endif
{

  public:

    MultiSliceContext( MultiSliceDicomImage& dcmImage, 
                       const std::vector< int32_t >& indexLut,
                       const std::vector< int32_t >& selection );

    void doIt( int32_t startIndex, int32_t count );

  private:

    MultiSliceDicomImage& _dicomImage;
    const std::vector< int32_t >& _indexLut;
    const std::vector< int32_t >& _selection;

};


}


#endif
