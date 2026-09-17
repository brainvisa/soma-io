
#ifndef CARTOBASE_THREAD_MUTEXPRIVATEPTHREAD_H
#define CARTOBASE_THREAD_MUTEXPRIVATEPTHREAD_H

// #ifndef _WIN32
#include <cartobase/config/cartobase_config.h>
#include <cartobase/thread/mutex.h>
#include <pthread.h>


namespace carto
{

  /** Private structure, specialized for pthread implementation only.
      Only Mutex and ThreadCondition implementations for pthread should
      use this structure */
  struct Mutex::Private
  {
    pthread_mutex_t	mutex;
    pthread_mutexattr_t	attributes;
  };
}

#endif
// #endif
