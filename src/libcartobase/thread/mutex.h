
#ifndef CARTOBASE_THREAD_MUTEX_H
#define CARTOBASE_THREAD_MUTEX_H

#include <cartobase/config/cartobase_config.h>

#ifndef CARTO_NO_THREAD


namespace carto
{

  class Mutex
  {
  public:
    enum Type
      {
        Fast = 0, 
        ProcessPrivate = 0, 
        ErrorChecking = 1, 
        Recursive = 2, 
        TypeMask = 3, 
        ProcessShared = 4, 
        SharedMask = 4, 
      };

    Mutex( int x = Fast );
    virtual ~Mutex();

    void lock();
    void unlock();

    void setProcessPrivate();
    void setProcessShared();
    void setRecursive();
    void setFast();
    void setErrorChecking();
    void setType( int x );

  private:

    friend class ThreadCondition;
    struct Private;

    Private *d;

    /// ThreadCondition class needs internal access to private structure
    Private *priv();
  };

}


#endif
#endif
