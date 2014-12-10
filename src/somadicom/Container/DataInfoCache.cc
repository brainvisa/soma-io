#include <soma-io/Container/DataInfoCache.h>


soma::DataInfoCache::DataInfoCache()
                   : soma::Singleton< soma::DataInfoCache >()
{
}


soma::DataInfo& soma::DataInfoCache::getDataInfo()
{

  return m_info;

}


void soma::DataInfoCache::clear()
{

  m_info.clear();

}

