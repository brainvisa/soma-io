
#include <cartobase/thread/semaphore.h>
#ifdef _WIN32
#include <windows.h>
#include <cartobase/uuid/uuid.h>
#else
#include <semaphore.h>
#include <errno.h>
#endif
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#if defined( __APPLE__ )
// another bug in MacOS: sem_init doesn't work and just sets 
// errno to 'function not implemented'... so unnamed semaphores 
// are just useless
// **WARNING:** sem_get doesn't work either, so Semaphore::getValue() 
// doesn't work.
#define CARTO_USE_NAMED_SEM
#endif
#ifdef CARTO_USE_NAMED_SEM
#include <cartobase/stream/sstream.h>
#endif

using namespace carto;
using namespace std;

struct Semaphore::Private
{
#ifdef _WIN32
  HANDLE	sem;
#else
  sem_t         *sem;
#ifdef CARTO_USE_NAMED_SEM
  string        semname;
#else
  sem_t		semx;
#endif
#endif
};


Semaphore::Semaphore( int num ) : d( new Semaphore::Private )
{
#ifdef _WIN32
  carto::UUID	uuid;
  uuid.generate();
  d->sem = CreateSemaphore( 0, num, 0x7fffffff, uuid.toString().c_str() );
#else
#ifdef CARTO_USE_NAMED_SEM
  static int    semcount = 0;
  int pid = getpid();
  do
  {
    ostringstream ss;
    ss << "cartosem_" << pid << "_" << semcount;
    ++semcount;
    d->semname = ss.str();
    errno = 0;
    d->sem = sem_open( d->semname.c_str(), O_CREAT | O_EXCL, 0600, num );
  }
  while( errno != 0 );
#else
  d->sem = &d->semx;
  if( errno != 0 )
    perror( "before sem_init" );
  sem_init( d->sem, 0, num );
  if( errno != 0 )
    perror( "after sem_init" );
  if( getValue() != num )
  {
    std::cerr << "BUG in sem_init/sem_open: value is " << getValue() 
      << " instead of " << num << std::endl << std::flush;
    if( errno != 0 )
      perror( 0 );
    else std::cerr << "errno is nul" << std::endl << std::flush;
    throw std::runtime_error( "semaphore initialization failure" );
  }
#endif
#endif
}


Semaphore::~Semaphore()
{
#ifdef _WIN32
  CloseHandle( d->sem );
#else
#ifdef CARTO_USE_NAMED_SEM
  if( d->sem )
    sem_unlink( d->semname.c_str() );
#else
  sem_destroy( d->sem );
#endif
#endif

  delete d;
}


bool Semaphore::post()
{
#ifdef _WIN32
  return ReleaseSemaphore( d->sem, 1, 0 );
  //return WaitForSingleObject( d->sem, INFINITE ) == WAIT_OBJECT_0;
#else
  return !sem_post( d->sem );
#endif
}


void Semaphore::wait()
{
#ifdef _WIN32
  WaitForSingleObject( d->sem, INFINITE );
#else
  sem_wait( d->sem );
#endif
}


bool Semaphore::tryWait()
{
#ifdef _WIN32
  return WaitForSingleObject( d->sem, 0 ) == WAIT_OBJECT_0;
#else
  return !sem_trywait( d->sem );
#endif
}


int Semaphore::getValue()
{
#ifdef _WIN32

  // NOT SAFE...
  if( !tryWait() )
    return 0;
  LONG	sval;
  ReleaseSemaphore( d->sem, 1, &sval );
  return sval;

#else

  int	sval;
  sem_getvalue( d->sem, &sval );
  return sval;

#endif
}


