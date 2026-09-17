#include <cartobase/smart/scopedptr.h>
#include <iostream>
#include <cartobase/exception/assert.h>


int main() 
{

  std::cout << "testing scoped_ptr default constructor..." << std::endl;
  {
    carto::scoped_ptr<int> x1, x2;
    ASSERT( !x1 );
    ASSERT( !x2 );
    ASSERT( x1 == 0 );
    ASSERT( x2 == 0 );
    ASSERT( x1 == x2 );
  }

  std::cout << "testing scoped_ptr constructor from pointer..." << std::endl;
  {
    carto::scoped_ptr<int> x1( new int );
    carto::scoped_ptr<int> x2( new int );
    ASSERT( x1 );
    ASSERT( x2 );
    ASSERT( x1 != 0 );
    ASSERT( x2 != 0 );
    ASSERT( x1 != x2 );
  }

  std::cout << "testing scoped_ptr constructor from std::unique_ptr..." << std::endl;
  {
    carto::scoped_ptr<int> x( std::unique_ptr<int>( new int ) );
    ASSERT( x != 0 );
  }

  std::cout << "testing scoped_ptr::reset()..." << std::endl;
  {
    carto::scoped_ptr<int> x1;
    ASSERT( x1 == 0 );
    x1.reset( new int );
    ASSERT( x1 != 0 );
    x1.reset();
    ASSERT( x1 == 0 );
  }
}
