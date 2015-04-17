#ifdef SOMA_IO_DICOM
#include <soma-io/Container/DataInfoCache.h>
#else
#include <Container/DataInfoCache.h>
#endif


dcm::DataInfoCache::DataInfoCache()
                  : dcm::Singleton< dcm::DataInfoCache >()
{
}


dcm::DataInfo& dcm::DataInfoCache::getDataInfo()
{

  return _info;

}


void dcm::DataInfoCache::clear()
{

  _info.clear();

}
