#ifndef CARTOBASE_THREAD_THREADCONDITION_H
#define CARTOBASE_THREAD_THREADCONDITION_H

#ifndef CARTO_NO_THREAD

#include <cartobase/config/cartobase_config.h>
#include <cartobase/thread/mutex.h>
#include <time.h>


namespace carto
{


class ThreadCondition
{

  public:

    ThreadCondition();
    virtual ~ThreadCondition();

    void wait( Mutex& mutex );
    void timedWait( Mutex& mutex, const struct timespec* t );
    void signal();
    void broadcast();

    void setProcessPrivate();
    void setProcessShared();

  private:

    struct Private;

    Mutex _mutex;
    Private *d;

};


}


#endif
#endif
