#ifndef _DataInfoCache_h_
#define _DataInfoCache_h_

#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Pattern/Singleton.h>
#include <soma-io/Container/DataInfo.h>
#else
#include <Pattern/Singleton.h>
#include <Container/DataInfo.h>
#endif


namespace dcm
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

    DataInfo _info;

};


}


#endif

