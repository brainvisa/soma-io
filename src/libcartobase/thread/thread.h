
#ifndef CARTOBASE_THREAD_THREAD_H
#define CARTOBASE_THREAD_THREAD_H

#include <cartobase/config/cartobase_config.h>

#ifndef CARTO_NO_THREAD


namespace carto
{


  /**	Short usage:
        - inherit Thread
        - define the doRun() function: this is the threaded job
        - run it by calling launch()
   */
  class Thread
  {

  public:

    Thread();
    virtual ~Thread();

    /// This is the function which should actually called to run a thread
    virtual void launch();

    /// Call launch(), not start()
    virtual void start();
    /// Call launch(), not run()
    virtual void run();
    virtual void stop();
    virtual void reset();

    virtual void join();
    virtual void detach();
    /** In suicide-safe mode, Thread can be deleted (or auto-deleted) while the
        threaded job is still running: doPostLaunch() and stop() are not 
        executed.
        Warning: it doesn't imply the thread is detached (if not, you may have
        to join() it to cleanup).
    */
    void setSuicideSafe( bool x = true );
    bool isSuicideSafe() const;

    virtual void setCancellable( bool value, bool callStop );
    virtual void cancel();

    virtual bool isActive() const;
    virtual bool isCurrent() const;
    static bool currentIsMainThread();

    static void* bootstrap( void* thread );

  protected:

    virtual void testCancel();
    virtual void doStart();
    virtual void doRun() = 0;
    virtual void doStop();
    virtual void doPreLaunch();
    virtual void doPostLaunch();

    bool _detached;
    bool _cancellable;
    bool _callStop;

  private:

    struct Private;
    Private *d;
  };

}


#endif
#endif
