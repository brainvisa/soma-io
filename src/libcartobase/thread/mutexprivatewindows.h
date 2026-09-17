
#ifndef CARTOBASE_THREAD_MUTEXPRIVATEWINDOWS_H
#define CARTOBASE_THREAD_MUTEXPRIVATEWINDOWS_H

#ifdef _WIN32
#include <cartobase/config/cartobase_config.h>
#include <cartobase/thread/mutex.h>
#include <windows.h>

namespace carto
{
  /** Private structure, specialized for windows implementation only.
      Only Mutex and ThreadCondition implementations for windows should
      use this structure */
  struct Mutex::Private
  {
    HANDLE	mutex;
  };

}

#endif
#endif
