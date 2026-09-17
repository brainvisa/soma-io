
#include <cartobase/thread/loopContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <cartobase/thread/thread.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace carto;

class FooContext : public LoopContext
{

  public:

    FooContext( /* put here the parameters of the algorithm */ )
              : LoopContext() { }
    ~FooContext() { }

    void doIt( int startIndex, int count )
    {

      for ( int i = startIndex; i < startIndex + count; i++ )
      {
#ifdef _WIN32
        Sleep( 1000 );
#else
        sleep( 1 );
#endif
	lock();
        std::cerr << " index = " << i
                  << std::endl;
	unlock();

      }

    }

  private:

    // private parameters of the algorithm

};


int main( int /*argc*/, char* /*argv*/[] )
{


  FooContext fooContext/* (my parameters for the algorithm )*/;
  ThreadedLoop
    threadedLoop( &fooContext,
                  3,  // the start index of the global loop
                  15, // the count to be done
                  0, // number of threads to use (0=number of physical CPUs)
                  2.0f   // 2 threads are launched on each CPU
                );

  // That will give, for instance, on a single processor computer:
  // 1 thread does iterations i=3,4,5,6,7.
  // Parallelly, 1 thread does iterations i=8,9,10,11,12.
  // The first of the threads that has finished realizes the 5 last iterations
  // i=13,14,15,16,17.
  // As a result, 2 threads are created on that single-processor computer.
  
  // As for a computer doted with 2 processors:
  // 3 threads are created; 2 threads on the processor I that deals with
  // iterations i=3,4,5,6,7 and i=13,14,15,16,17, and 1 thread on 
  // processor II that deals with iterations i=8,9,10,11,12
  
  // Typically, one should use 1 thread per CPU, and one should use a unitary
  // grain in image processing algorithms; so we often have to implement
  // a threaded loop of the following type:
  //   pguitk::ThreadedLoop loop( &myAlgoContext, 1, startIter, count, 1 );
		   
  threadedLoop.launch(); // start the loop process

  return EXIT_SUCCESS;

}
