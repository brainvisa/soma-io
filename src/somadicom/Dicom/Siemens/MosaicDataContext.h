#ifndef _MosaicDataContext_h_
#define _MosaicDataContext_h_


#include <soma-io/config/soma_config.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>

#include <string>
#include <vector>


namespace soma
{


class DicomProxy;
class Demosaicer;
class Callback;


class MosaicDataContext : public carto::LoopContext
{

  public:

    MosaicDataContext( const std::vector< std::string >& files,
                       DicomProxy& proxy,
                       Demosaicer& demosaicer,
                       Callback* progress = 0 );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const std::vector< std::string >& m_files;
    DicomProxy& m_proxy;
    Demosaicer& m_demosaicer;
    Callback* m_progress;
    int32_t m_count;

};


}


#endif
