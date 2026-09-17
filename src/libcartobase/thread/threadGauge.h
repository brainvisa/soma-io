
#ifndef CARTOBASE_THREAD_THREADGAUGE_H
#define CARTOBASE_THREAD_THREADGAUGE_H

#include <cartobase/config/cartobase_config.h>

#ifndef CARTO_NO_THREAD

namespace carto
{

  class ThreadGauge
  {

  public:

    ThreadGauge();
    virtual ~ThreadGauge();

    virtual void add( int );
    virtual void reset();
  };

}


#endif
#endif
