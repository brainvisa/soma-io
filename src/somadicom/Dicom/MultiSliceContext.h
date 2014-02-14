#ifndef _MultiSliceContext_h_
#define _MultiSliceContext_h_


#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmimgle/dcmimage.h>


namespace soma
{


class Data;
class Callback;


class MultiSliceContext : public carto::LoopContext
{

  public:

    MultiSliceContext( DicomImage& dcmImage, 
                       Data& data,
                       Callback* progress = 0 );

    void doIt( int32_t startIndex, int32_t count );

  private:

    DicomImage& m_dcmImage;
    Data& m_data;
    Callback* m_progress;
    int32_t m_count;

};


}


#endif
