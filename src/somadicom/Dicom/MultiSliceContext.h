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

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>

#include <vector>


namespace soma
{


class DicomProxy;


class MultiSliceContext : public LoopContext
{

  public:

    MultiSliceContext( DicomImage& dcmImage, 
                       DicomProxy& proxy,
                       const std::vector< int32_t >& indexLut,
                       const std::vector< int32_t >& selection );

    void doIt( int32_t startIndex, int32_t count );

  private:

    DicomImage& _dcmImage;
    DicomProxy& _proxy;
    const std::vector< int32_t >& _indexLut;
    const std::vector< int32_t >& _selection;

};


}


#endif
