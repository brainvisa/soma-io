#ifndef CARTO_NO_THREAD

#include <cartobase/thread/thread.h>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

using namespace std;

#ifdef _WIN32
static DWORD mainThreadId;
#else
static pthread_t mainThreadId = 0;
#endif
static bool mainThreadInitialized = false;

namespace carto
{

  struct Thread::Private
  {
    Private() : suicideSafe( false ) {}

#ifdef _WIN32
    HANDLE		thread;
    DWORD		threadId;
#else
    pthread_t		thread;
    pthread_attr_t	attributes;
#endif
    bool		suicideSafe;
  };

}


carto::Thread::Thread()
             : _detached( false ),
               _cancellable( false ),
               _callStop( false ), 
               d( new carto::Thread::Private )
{
  if( !mainThreadInitialized )
    {
      /* the first time we are called, we can be called only from the 
	 main thread (unless the system API has been used directly) */
#ifdef _WIN32
      mainThreadId = GetCurrentThreadId();
#else
      mainThreadId = pthread_self();
#endif
      mainThreadInitialized = true;
    }



#ifdef CARTO_DEBUG

  std::cerr << "Thread::Thread()" << std::endl;

#endif

  reset();
#ifndef _WIN32
  if ( pthread_attr_init( &d->attributes ) )
    std::cerr << "Thread attributes initialization failed"
              << std::endl;
  // Windows: nothing to be done here
#endif
}


carto::Thread::~Thread()
{

#ifdef CARTO_DEBUG

  std::cerr << "Thread::~Thread()" << std::endl;

#endif
#ifndef _WIN32
  pthread_attr_destroy( &d->attributes );
#endif
  delete d;
}


void carto::Thread::start()
{

#ifdef CARTO_DEBUG

  std::cerr << "Thread::start()" << std::endl;

#endif

  doStart();

}


void carto::Thread::run()
{

#ifdef CARTO_DEBUG

  std::cerr << "Thread::run()" << std::endl;

#endif

  doRun();

}


void carto::Thread::stop()
{

#ifdef CARTO_DEBUG

  std::cerr << "Thread::stop()" << std::endl;

#endif

  doStop();

}


void carto::Thread::reset()
{

#ifdef CARTO_DEBUG

  std::cerr << "Thread::reset()" << std::endl;

#endif

  d->thread = 0;
#ifdef _WIN32
  d->threadId = 0;
#endif
}


#ifdef _WIN32
namespace
{
  DWORD WINAPI _bootstrap( PVOID thread )
  {
    void *x = carto::Thread::bootstrap( thread );
    // void* is of type size_t which is variable among 64-bit systems and
    // not always compatible with DWORD on windows 64. Moreover, bootstrap 
    // always return 0
    //return (DWORD) x;
    return 0;
  }
}
#endif


void carto::Thread::launch()
{
 
#ifdef CARTO_DEBUG

  std::cerr << "Thread::launch()" << std::endl;

#endif

  doPreLaunch();
  bool suic = d->suicideSafe;

#ifdef _WIN32
  // TODO: take attributes into account...
  d->thread = CreateThread( NULL, 0, _bootstrap, (PVOID) this, 0, &d->threadId );
  if( !d->thread )
    {
      cerr << "Thread::launch() : failed." << endl;
      return;
    }
  if( _detached )
    CloseHandle( d->thread );
#else
  int	err = pthread_create( &d->thread, &d->attributes, bootstrap, this );
  if ( err )
    std::cerr << "Thread::launch() : failed. code: " << err << ", thread: " 
              << d->thread << std::endl;
#endif
  if( !suic )
    doPostLaunch();

}


void carto::Thread::join()
{

#ifdef CARTO_DEBUG

  std::cerr << "Thread::join()" << std::endl;

#endif

#ifdef _WIN32
  if( WaitForSingleObject( d->thread, INFINITE ) != WAIT_OBJECT_0 )
    cerr << "Thread::join() failed!" << endl;
  else
    CloseHandle( d->thread );
#else
  pthread_join( d->thread, 0 );
#endif

  reset();
}


void carto::Thread::detach()
{

#ifdef CARTO_DEBUG

  std::cerr << "Thread::detach()" << std::endl;

#endif

#ifdef _WIN32
  if( d->thread && !_detached )
    CloseHandle( d->thread );
#else
  if ( pthread_attr_setdetachstate( &d->attributes, PTHREAD_CREATE_DETACHED ) )
  {

    _detached = false;
    std::cerr << "  Failed to set thread detached mode"
              << std::endl;

  }
  else
    _detached = true; 
#endif
}


void carto::Thread::setCancellable( bool value, bool callStop )
{

#ifdef CARTO_DEBUG

  std::cerr << "Thread::setCancellable()" << std::endl;

#endif

  _cancellable = value;
  _callStop = callStop;
#ifdef _WIN32
  //if( d->thread )
  //  SetHandleInformation( d->thread, THREAD_TERMINATE, );
  // TODO
  // It seems cancel state does not exist on Windows
#else
  if ( value )
    pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, 0 );
#endif
}


void carto::Thread::cancel()
{

#ifdef CARTO_DEBUG

  std::cerr << "Thread::cancel()" << std::endl;

#endif

#ifdef _WIN32
  if( !TerminateThread( d->thread, 0 ) )
#else
  if ( pthread_cancel( d->thread ) )
#endif
    std::cerr << "  Thread failed in cancellation"
              << std::endl;
  else
  {

    if ( _callStop )
      stop(); 
    reset();

  }

}


bool carto::Thread::isActive() const
{

  return d->thread != 0;

}


void carto::Thread::setSuicideSafe( bool x )
{
  d->suicideSafe = x;
}


bool carto::Thread::isSuicideSafe() const
{
  return d->suicideSafe;
}


void* carto::Thread::bootstrap( void* thread )
{

#ifdef CARTO_DEBUG

  std::cerr << "Thread::bootstrap()" << std::endl;

#endif

  carto::Thread* theThread = static_cast< carto::Thread* >( thread );

  if ( theThread )
  {

    theThread->start();
    bool	suic = theThread->isSuicideSafe();
    theThread->run();
    if( !suic )
      theThread->stop();

  }

  return 0;

}


void carto::Thread::testCancel()
{

#ifdef CARTO_DEBUG

  std::cerr << "Thread::testCancel()" << std::endl;

#endif
#ifndef _WIN32
  if ( _cancellable )
    pthread_testcancel();
#endif
}


void carto::Thread::doStart()
{
}


void carto::Thread::doStop()
{
}


void carto::Thread::doPreLaunch()
{
}


void carto::Thread::doPostLaunch()
{
}


bool carto::Thread::isCurrent() const
{
#ifdef _WIN32
  return GetCurrentThreadId() == d->threadId;
#else
  return pthread_equal(pthread_self(), d->thread) ? true : false;
#endif
}


bool carto::Thread::currentIsMainThread()
{
  if( !mainThreadInitialized )
    return true;

#ifdef _WIN32
  return GetCurrentThreadId() == mainThreadId;
#else
  return pthread_equal(pthread_self(), mainThreadId) ? true : false;
#endif
}


#endif
