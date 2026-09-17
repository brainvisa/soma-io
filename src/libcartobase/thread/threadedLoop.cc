#ifndef CARTO_NO_THREAD

#include <cartobase/thread/threadedLoop.h>
#include <cartobase/thread/loopThread.h>
#include <cartobase/thread/loopContext.h>
#include <cartobase/thread/semaphore.h>
#include <cartobase/thread/cpu.h>
#include <list>
#include <iostream>
#include <math.h>


struct carto::ThreadedLoop::Private
{
  Private();
  ~Private();

  bool 				endThreads;
  std::list<LoopThread *>	threads;
};


carto::ThreadedLoop::Private::Private()
  : endThreads( true )
{
}


carto::ThreadedLoop::Private::~Private()
{
  std::list<LoopThread *>::iterator	i, e = threads.end();
  LoopThread				*lt;
  for( i=threads.begin(); i!=e; ++i )
    {
      lt = *i;
      lt->setFinished( true );
      lt->holdSemaphore()->post();
      lt->join();
      delete lt;
    }
}


carto::ThreadedLoop::ThreadedLoop( carto::LoopContext* loopContext,
                                   int startIndex, int count,
                                   int maxThreadCount,
                                   float threadsByCpu )

  : d( new Private ), 
    _loopContext( loopContext ),
    _grain( 0 ),
    _startIndex( startIndex ),
    _count( count ),
    _maxThreadCount( maxThreadCount ),
    _threadsByCpu( threadsByCpu ),
    _currentIndex( startIndex ),
    _todo( count )
{
}


carto::ThreadedLoop::~ThreadedLoop()
{
  delete d;
}



void carto::ThreadedLoop::setLoopContext( LoopContext* lc )
{

  _loopContext = lc;

}


void carto::ThreadedLoop::run()
{

  if ( _loopContext )
  {

    _loopContext->lock();

    if ( ( _todo <= 0 ) || ( _grain < 1 ) || _loopContext->cancel() )
    {

      _loopContext->unlock();
      if ( _loopContext->cancel() )
        std::cerr << "=========================> aborted by cancel" << std::endl;

#ifdef CARTO_DEBUG

    std::cerr << "  ThreadedLoop::getStartIndexAndCount() "
              << "return false" << std::endl;

#endif

    }
    else
    {

      int startIndex = _currentIndex;
      int count = _grain;

      _todo -= _grain;

      if ( _remain )
      {

        count++;
        _todo--;
        _remain--;

      }

      _currentIndex += count;

#ifdef CARTO_DEBUG

      std::cerr << "  ThreadedLoop::getStartIndexAndCount() "
                << "{ start=" << startIndex << ", count=" << count << " }"
	        << std::endl;

#endif

      _loopContext->gaugeAdd( count );
      _loopContext->unlock();
      _loopContext->doIt( startIndex, count );

#ifdef CARTO_DEBUG

      std::cerr << "  ThreadedLoop::getStartIndexAndCount() "
                << "return true" << std::endl;

#endif

    }

  }

}


bool carto::ThreadedLoop::launch( bool resetGauge, bool resetCancel )
{

  bool result = true;

  unsigned nCpu = cpuCount();

  if ( _maxThreadCount >= 1
       && static_cast<unsigned>(_maxThreadCount) < nCpu )
  {
    nCpu = _maxThreadCount;
  }
  int nth = static_cast<int>( rint( nCpu * _threadsByCpu ) );
  _loopContext->setSingleThreaded( nth == 1 );

  _loopContext->unlock();
  _loopContext->lock();


#ifdef CARTO_DEBUG

  std::cerr << "CPU count     : " << nCpu << std::endl;
  std::cerr << "thread by CPU : " << _threadsByCpu << std::endl;

#endif

  int		i,
    n = d->threads.size();
  LoopThread	*lt;
  Semaphore	*sem;

  if( nth < 1 )
    nth = 1;

  _currentIndex = _startIndex;
  _todo = _count;
  _grain = ( _count <= nth ) ? 1 : _count / nth;
  _remain = ( _count <= nth ) ? 0 : _count % nth;

  for( i=nth; i<n; ++i )
    {
      lt = d->threads.back();
      d->threads.pop_back();
      if( !endThreadsWhenDone() )
        {
          sem = lt->holdSemaphore();
          lt->setFinished( true );
          // release thread
          sem->post();
          // let is finish
          lt->join();
        }
      delete lt;
    }

  if( nth == 1 )
    return runSingleThreaded(resetGauge, resetCancel );

  int countThread = 0;
  int t;
  n = d->threads.size();

  for ( t = n; ( t < nth ) &&  ( t * _grain < _count ); 
        ++t, ++countThread )
    {
      lt = new carto::LoopThread( this );
      d->threads.push_back( lt );
      if( !endThreadsWhenDone() )
        lt->launch();
    }

#ifdef CARTO_DEBUG

  std::cerr << "thread count  : " << countThread << std::endl;

#endif

  std::list<LoopThread *>::iterator	it, et = d->threads.end();

  for ( it = d->threads.begin(); it != et; ++it )
    {
      lt = *it;
      if( endThreadsWhenDone() )
        lt->launch();
      else
        lt->holdSemaphore()->post(); // start
    }

  _loopContext->unlock();

  if( endThreadsWhenDone() )
    {
      for ( it = d->threads.begin(); it != et; ++it )
        {
          lt = *it;
          lt->join();
          delete lt;
        }
      d->threads.clear();
    }
  else
    for ( it = d->threads.begin(); it != et; ++it )
      (*it)->endSemaphore()->wait();

  if ( _loopContext->cancel() )
    result = false;

  if ( resetGauge )
    _loopContext->resetGauge();
  if ( resetCancel )
    _loopContext->resetCancel();

  return result;

}


bool carto::ThreadedLoop::runSingleThreaded( bool resetGauge,
                                             bool resetCancel )
{
  _loopContext->doIt( _startIndex, _count );
  if ( resetGauge )
    _loopContext->resetGauge();
  if ( resetCancel )
    _loopContext->resetCancel();

  _loopContext->setSingleThreaded( false );

  if ( _loopContext->cancel() )
    return false;
 return true;
}


void carto::ThreadedLoop::setEndThreadsWhenDone( bool x )
{
  d->endThreads = x;
}


bool carto::ThreadedLoop::endThreadsWhenDone() const
{
  return d->endThreads;
}


void carto::ThreadedLoop::setLoopCount( int n )
{
  _count = n;
}

#endif
