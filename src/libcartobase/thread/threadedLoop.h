
#ifndef CARTOBASE_THREAD_THREADEDLOOP_H
#define CARTOBASE_THREAD_THREADEDLOOP_H

#include <cartobase/config/cartobase_config.h>

#ifndef CARTO_NO_THREAD

namespace carto
{

  class LoopContext;


  class ThreadedLoop
  {

  public:

    ThreadedLoop( LoopContext* loopContext,
                  int startIndex, int count, int maxThreadCount = 0,
                  float threadsByCpu = 1.0f );
    virtual ~ThreadedLoop();

    /** This function is only useful in multiple run mode (endThreadsWhenDone 
        false). Call it only between two runs, from the control thread (the 
        thread running the loop).
    */
    void setLoopContext( LoopContext* );

    /// run the loop to the end until all iterations are finished
    bool launch( bool resetGauge = true, bool resetCancel = true );
    /// run one iteration step. It normally calls the LoopContext diIt() method
    void run();

    /** Warning: it's possibly not safe to change this value from false to true
        after launch() has been called */
    void setEndThreadsWhenDone( bool x );
    /** The loop can work in two modes: single run (endThreadsWhenDone() is 
        true) or multiple runs (endThreadsWhenDone is false).
        - In single run mode, launch() creates the needed number of threads, 
        runs them to complete the loop, then destroys every thread.
        - In multiple runs mode, the first call to launch() creates the needed 
        threads, but doesn't destroy them after the loop is finished. Each 
        thread stays alive and blocks on a semaphore. When you call launch() 
        again later for another run (possibly after changing the LoopContext), 
        the existing threads will be re-used for the next loop, so the cost of 
        destroying and creating threads again is avoided. All threads will be 
        actually destroyed when the ThreadedLoop is deleted.
     */
    bool endThreadsWhenDone() const;
    void setLoopCount( int n );
    bool runSingleThreaded( bool resetGauge = true, bool resetCancel = true );

  private:
    struct Private;

    Private	*d;

    LoopContext* _loopContext;
    int _grain;
    int _startIndex;
    int _count;
    int _maxThreadCount;
    float _threadsByCpu;

    int _currentIndex;
    int _todo;
    int _remain;

  };

}


#endif
#endif
