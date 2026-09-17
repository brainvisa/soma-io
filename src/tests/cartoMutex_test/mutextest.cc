
#include <cstdlib>
#include <cartobase/thread/thread.h>
#include <cartobase/thread/mutex.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace carto;
using namespace std;

Mutex mutex_g;

// #ifdef _WIN32
// void sleep( unsigned ns )
// {
  // _sleep( ns * 1000 );
// }
// #endif

class MyThread : public Thread
{
public:
  MyThread( int id ) : Thread(), _id( id ) {}
  virtual ~MyThread() {}
  virtual void doRun();
  int _id;
};

void MyThread::doRun()
{
  cout << "doRun in thread " << _id << endl << flush;
  sleep( 1 );
  mutex_g.lock();
  cout << "mutex locked for thread " << _id << endl << flush;
  sleep( 1 );
  cout << "mutex unlocked for thread " << _id << endl << flush;
  mutex_g.unlock();
}


int main( int /*argc*/, char* /*argv*/[] )
{
  MyThread    t1( 1 ), t2( 2 ), t3( 3 );
  cout << "running threads\n" << flush;
  t1.launch();
  t2.launch();
  t3.launch();
  cout << "(main) threads running...\n" << flush;
  t1.join();
  t2.join();
  t3.join();
  cout << "threads finished\n" << flush;
  return EXIT_SUCCESS;
}

