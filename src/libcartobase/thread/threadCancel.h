
#ifndef CARTOBASE_THREAD_THREADCANCEL_H
#define CARTOBASE_THREAD_THREADCANCEL_H

#include <cartobase/config/cartobase_config.h>

#ifndef CARTO_NO_THREAD


namespace carto
{

  class ThreadCancel
  {

  public:

    ThreadCancel();
    virtual ~ThreadCancel();

    virtual bool check();
    virtual void reset();
  };

}


#endif
#endif
