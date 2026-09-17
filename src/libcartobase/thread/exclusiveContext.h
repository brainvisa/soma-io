
#ifndef CARTOBASE_THREAD_EXCLUSIVECONTEXT_H
#define CARTOBASE_THREAD_EXCLUSIVECONTEXT_H

#include <cartobase/config/cartobase_config.h>

#ifndef CARTO_NO_THREAD

#include <cartobase/thread/mutex.h>


namespace carto
{

  class ExclusiveContext
  {

  public:

    ExclusiveContext( bool singlethread = false );
    virtual ~ExclusiveContext();

    void lock();
    void unlock();
    void setSingleThreaded( bool singlethread )
    { _singlethread = singlethread; }
    bool isSingleThreaded() const { return _singlethread; }

  private:

    bool _singlethread;
    Mutex _mutex;
  };

}


#endif
#endif
