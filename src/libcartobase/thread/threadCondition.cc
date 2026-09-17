#ifndef CARTO_NO_THREAD

#include <cartobase/thread/threadCondition.h>
#include <cartobase/thread/mutex.h>
#ifdef _WIN32
#include <cartobase/thread/mutexprivatewindows.h>
#else
#include <cartobase/thread/mutexprivatepthread.h>
#endif
#include <iostream>

using namespace std;

namespace carto
{
  struct ThreadCondition::Private
  {
#ifdef _WIN32

    HANDLE		condition;

#else

    pthread_cond_t	condition;
    pthread_condattr_t	attributes;

#endif
 };
}


carto::ThreadCondition::ThreadCondition() : d( new ThreadCondition::Private )
{
#ifdef _WIN32

  // TODO
  cerr << "ThreadCondition: not implemented for Windows" << endl;

#else

  pthread_cond_init( &d->condition, 0 );
  pthread_condattr_init( &d->attributes );

#endif
}


carto::ThreadCondition::~ThreadCondition()
{
#ifdef _WIN32

  // TODO

#else

  pthread_condattr_destroy( &d->attributes );
  pthread_cond_destroy( &d->condition );

#endif

  delete d;
}


void carto::ThreadCondition::wait( carto::Mutex& mutex )
{

  _mutex.lock();

#ifdef _WIN32
  // TODO
#else
  pthread_cond_wait( &d->condition, &mutex.priv()->mutex );
#endif

  _mutex.unlock();

}


void carto::ThreadCondition::timedWait( carto::Mutex& mutex,
                                       const struct timespec* t )
{

  _mutex.lock();

#ifdef _WIN32
  // TODO
#else
  pthread_cond_timedwait( &d->condition, &mutex.priv()->mutex, t );
#endif

  _mutex.unlock();

}


void carto::ThreadCondition::signal()
{
#ifdef _WIN32
  // TODO
#else
  pthread_cond_signal( &d->condition );
#endif
}


void carto::ThreadCondition::broadcast()
{
#ifdef _WIN32
  // TODO
#else
  pthread_cond_broadcast( &d->condition );
#endif
}


void carto::ThreadCondition::setProcessPrivate()
{
#ifdef _WIN32
  // TODO
#else
#if !defined( __GLIBC__ ) || __GLIBC_PREREQ(2,2)
  if ( pthread_condattr_setpshared( &d->attributes, PTHREAD_PROCESS_PRIVATE ) )
    std::cerr << "carto::Condition : failed in setting private mode"
              << std::endl;
#else
#warning "carto::Mutex::setProcessPrivate() won't work, needs glibc >= 2.2"
#endif
#endif
}


void carto::ThreadCondition::setProcessShared()
{
#ifdef _WIN32
  // TODO
#else
#if !defined( __GLIBC__ ) || __GLIBC_PREREQ(2,2)
  if ( pthread_condattr_setpshared( &d->attributes, PTHREAD_PROCESS_SHARED ) )
    std::cerr << "carto::Condition : failed in setting shared mode"
              << std::endl;
#else
#warning "carto::Mutex::setProcessPrivate() won't work, needs glibc >= 2.2"
#endif
#endif
}

#endif
