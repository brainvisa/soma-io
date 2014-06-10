#ifndef _MultiFrameContext_h_
#define _MultiFrameContext_h_


#include <soma-io/config/soma_config.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>


namespace soma
{


class Data;
class Callback;


class MultiFrameContext : public carto::LoopContext
{

  public:

    MultiFrameContext( DicomImage& dcmImage, 
                       Data& data,
                       bool fillSlices = false,
                       Callback* progress = 0 );

    void doIt( int32_t startIndex, int32_t count );

  private:

    DicomImage& m_dcmImage;
    Data& m_data;
    bool m_fillSlices;
    Callback* m_progress;
    int32_t m_count;

};


}


#endif
