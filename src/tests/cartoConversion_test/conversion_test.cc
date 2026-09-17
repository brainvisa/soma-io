#include <cartobase/type/conversion.h>
#include <cartobase/smart/rcobject.h>
#include <cartobase/exception/assert.h>

class Foo : public carto::RCObject
{

  public:

    ~Foo();

};


Foo::~Foo()
{
}


template < bool B >
struct test
{
};


template <>
struct test< false >
{

  static const bool test_TRUE = false;

};


template <>
struct test< true >
{

  static const bool test_TRUE = true;

};


int main()
{

  ASSERT( test< SUPERSUBCLASS( carto::RCObject, Foo ) >::test_TRUE );
  ASSERT( !test< SUPERSUBCLASS( Foo, carto::RCObject ) >::test_TRUE );
  ASSERT( !test< SUPERSUBCLASS( int, Foo ) >::test_TRUE );

}
