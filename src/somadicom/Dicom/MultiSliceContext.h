#ifndef _MultiSliceContext_h_
#define _MultiSliceContext_h_


#include <soma-io/config/soma_config.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>

#include <vector>


namespace soma
{


class DicomProxy;
class Callback;


class MultiSliceContext : public carto::LoopContext
{

  public:

    MultiSliceContext( DicomImage& dcmImage, 
                       DicomProxy& proxy,
                       const std::vector< int32_t >& indexLut,
                       Callback* progress = 0 );

    void doIt( int32_t startIndex, int32_t count );

  private:

    DicomImage& m_dcmImage;
    DicomProxy& m_proxy;
    const std::vector< int32_t >& m_indexLut;
    Callback* m_progress;
    int32_t m_count;

};


}


#endif
