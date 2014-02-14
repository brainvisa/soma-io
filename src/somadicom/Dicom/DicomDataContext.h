#ifndef _DicomDataContext_h_
#define _DicomDataContext_h_


#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>

#include <string>
#include <vector>


namespace soma
{


class Data;
class Callback;


class DicomDataContext : public carto::LoopContext
{

  public:

    DicomDataContext( const std::vector< std::string >& files,
                      Data& data,
                      bool applyModalityLUT = true,
                      Callback* progress = 0 );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const std::vector< std::string >& m_files;
    Data& m_data;
    Callback* m_progress;
    int32_t m_count;

};


}


#endif
