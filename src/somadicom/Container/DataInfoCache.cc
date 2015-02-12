#ifdef SOMA_IO_DICOM
#include <soma-io/Container/DataInfoCache.h>
#else
#include <Container/DataInfoCache.h>
#endif


soma::DataInfoCache::DataInfoCache()
                   : soma::Singleton< soma::DataInfoCache >()
{
}


soma::DataInfo& soma::DataInfoCache::getDataInfo()
{

  return _info;

}


void soma::DataInfoCache::clear()
{

  _info.clear();

}

