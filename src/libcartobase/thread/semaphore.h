
#ifndef CARTOBASE_THREAD_SEMAPHORE_H
#define CARTOBASE_THREAD_SEMAPHORE_H

#include <cartobase/config/cartobase_config.h>

#ifndef CARTO_NO_THREAD


namespace carto
{

  class Semaphore
  {

  public:

    Semaphore( int initnum = 0 );
    ~Semaphore();

    bool post();
    void wait();
    bool tryWait();
    int getValue();

  private:
    struct Private;

    Private *d;
  };

}


#endif
#endif
