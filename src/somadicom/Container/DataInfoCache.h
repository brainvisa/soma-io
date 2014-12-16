#ifndef _DataInfoCache_h_
#define _DataInfoCache_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Pattern/Singleton.h>
#include <soma-io/Container/DataInfo.h>


namespace soma
{


class DataInfoCache : public Singleton< DataInfoCache >
{

  public:

    DataInfo& getDataInfo();

    void clear();

  protected:

    friend class Singleton< DataInfoCache >;

    DataInfoCache();

  private:

    DataInfo m_info;

};


}


#endif

