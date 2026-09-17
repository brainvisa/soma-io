#ifndef CARTO_NO_THREAD

#include <cartobase/thread/loopThread.h>
#include <cartobase/thread/loopContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <cartobase/thread/semaphore.h>
#include <iostream>


struct carto::LoopThread::Private
{
  Private() : finished( false ), holdsem( 0 ), endsem( 0 ) {}
  ~Private();
  bool		finished;
  Semaphore	*holdsem;
  Semaphore	*endsem;
};


carto::LoopThread::Private::~Private()
{
  if( holdsem )
    {
      holdsem->post();
      delete holdsem;
    }
  if( endsem )
    {
      endsem->post();
      delete endsem;
    }
}



carto::LoopThread::LoopThread( carto::ThreadedLoop* parent )
                 : carto::Thread(),
                   _parent( parent ), 
                   d( 0 )
{
  if( !parent->endThreadsWhenDone() )
    {
      d = new Private;
      d->holdsem = new Semaphore( 0 );
      d->endsem = new Semaphore( 0 );
    }
}


carto::LoopThread::~LoopThread()
{

#ifdef CARTO_DEBUG

  std::cerr << "LoopThread::~LoopThread()" << std::endl;

#endif

  delete d;

}


void carto::LoopThread::doRun()
{

  bool single = _parent->endThreadsWhenDone();

  do
    {
      if( !single )
        {
          d->holdsem->wait(); // wait for start signal
          if( finished() )
            break;
        }

      // regular loop (one run)
      _parent->run();

      if( !single )
        d->endsem->post(); // signal the loop I'm done
    }
  while( !single );

}


bool carto::LoopThread::finished() const
{
  if( d )
    return d->finished;
  return true;
}


void carto::LoopThread::setFinished( bool x )
{
  if( d )
    d->finished = x;
}


carto::Semaphore* carto::LoopThread::holdSemaphore()
{
  if( d )
    return d->holdsem;
  return 0;
}


carto::Semaphore* carto::LoopThread::endSemaphore()
{
  if( d )
    return d->endsem;
  return 0;
}

#endif
