#ifndef CARTO_NO_THREAD

#include <cartobase/thread/exclusiveContext.h>


carto::ExclusiveContext::ExclusiveContext( bool singlethread )
  : _singlethread( singlethread )
{
}


carto::ExclusiveContext::~ExclusiveContext()
{
}


void carto::ExclusiveContext::lock()
{

  if( !_singlethread )
    _mutex.lock();

}


void carto::ExclusiveContext::unlock()
{

  if( !_singlethread )
    _mutex.unlock();

}

#endif
