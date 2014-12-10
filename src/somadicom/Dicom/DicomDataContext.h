#ifndef _DicomDataContext_h_
#define _DicomDataContext_h_


#include <soma-io/config/soma_config.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>

#include <string>
#include <vector>


namespace soma
{


class DicomProxy;
class Callback;


class DicomDataContext : public carto::LoopContext
{

  public:

    DicomDataContext( const std::vector< std::string >& files,
                      DicomProxy& proxy,
                      bool applyModalityLUT = true,
                      Callback* progress = 0 );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const std::vector< std::string >& m_files;
    DicomProxy& m_proxy;
    Callback* m_progress;
    int32_t m_count;

};


}


#endif
