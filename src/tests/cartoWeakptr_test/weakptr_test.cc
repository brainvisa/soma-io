#include <cartobase/smart/weakptr.h>
#include <cartobase/smart/weakobject.h>
#include <cartobase/exception/assert.h>

class Foo : public carto::WeakObject
{
  public:
    ~Foo();
};


Foo::~Foo()
{
}


int main()
{
  Foo* p1 = new Foo;
  carto::weak_ptr< Foo > x1( p1 );
  ASSERT( x1 );
  Foo* p2 = new Foo;
  carto::weak_ptr< Foo > x2;
  ASSERT( !x2 );
  ASSERT( x1 != x2 );
  x2.reset( p2 );
  ASSERT( x2 );
  x2.release();
  ASSERT( !x2 );
  x2 = x1;
  ASSERT( x2 );
  delete p2;
  ASSERT( x2 );
  delete p1;
  ASSERT( !x1 );
  ASSERT( !x2 );
}
