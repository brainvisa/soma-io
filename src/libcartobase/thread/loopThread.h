
#ifndef CARTOBASE_THREAD_LOOPTHREAD_H
#define CARTOBASE_THREAD_LOOPTHREAD_H

#include <cartobase/config/cartobase_config.h>

#ifndef CARTO_NO_THREAD

#include <cartobase/thread/thread.h>


namespace carto
{

  class ThreadedLoop;
  class Semaphore;


  class LoopThread : public Thread
  {

  public:

    LoopThread( ThreadedLoop* parent );
    virtual ~LoopThread();

    bool finished() const;
    void setFinished( bool x );
    /** the thread automatically locks on this semaphore at start and 
        at the beginning of each run (in multiple run mode). In single run 
        mode, 0 is returned
    */
    Semaphore* holdSemaphore();
    /** The ThreadedLoop locks on this semaphore to wait for a run end 
        (in multiple run mode) and is released by this ThreadLoop after each 
        run. In single run mode, 0 is returned
    */
    Semaphore* endSemaphore();

  protected:

    virtual void doRun();

    ThreadedLoop* _parent;

  private:
    struct Private;
    Private	*d;
  };

}


#endif
#endif
