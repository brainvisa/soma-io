#define private public
#include <cartobase/smart/rcptr.h>
#include <cartobase/smart/rcobject.h>
#include <cartobase/exception/assert.h>
#include <iostream>
#include <memory>


class Bar
{
  public:
    Bar();
    virtual ~Bar();
};


Bar::Bar()
{
  std::cout << ">>> " << this << std::endl;
}


Bar::~Bar()
{
  std::cout << "<<< " << this << std::endl;
}


class Foo : public carto::RCObject
{
  public:
    Foo();
    virtual ~Foo();
};


Foo::Foo()
{
  std::cout << ">>> " << this << std::endl;
}


Foo::~Foo()
{
  std::cout << "<<< " << this << std::endl;
}


carto::rc_ptr<Foo> function1();

carto::rc_ptr<Foo> function1()
{
  return carto::rc_ptr<Foo>( new Foo );
}


carto::rc_ptr<Foo> function2( const carto::rc_ptr<Foo>& p );

carto::rc_ptr<Foo> function2( const carto::rc_ptr<Foo>& p )
{
  return p;
}


carto::rc_ptr<Foo> function3( carto::rc_ptr<Foo> p );

carto::rc_ptr<Foo> function3( carto::rc_ptr<Foo> p )
{
  return p;
}


int main() 
{
  std::cout << "testing rc_ptr constructors (RCObject)..." << std::endl;
  {
    carto::rc_ptr<Foo> x1, x2;
    ASSERT( !x1 );
    ASSERT( !x2 );
    ASSERT( x1 == 0 );
    ASSERT( x2 == 0 );
    ASSERT( x1 == x2 );
  }
  std::cout << "testing rc_ptr constructors..." << std::endl;
  {
    carto::rc_ptr<Bar> x1, x2;
    ASSERT( !x1 );
    ASSERT( !x2 );
    ASSERT( x1 == 0 );
    ASSERT( x2 == 0 );
    ASSERT( x1 == x2 );
  }

  std::cout << "testing rc_ptr constructors (RCObject)..." << std::endl;
  {
    carto::rc_ptr<Foo> x1, x2;
    ASSERT( !x1 );
    ASSERT( !x2 );
    ASSERT( x1 == 0 );
    ASSERT( x2 == 0 );
    ASSERT( x1 == x2 );
  }
  std::cout << "testing rc_ptr constructors..." << std::endl;
  {
    carto::rc_ptr<Bar> x1, x2;
    ASSERT( !x1 );
    ASSERT( !x2 );
    ASSERT( x1 == 0 );
    ASSERT( x2 == 0 );
    ASSERT( x1 == x2 );
  }

  std::cout << "testing rc_ptr constructors from std::unique_ptr (RCObject)..." << std::endl;
  {
    carto::rc_ptr<Foo> x(std::unique_ptr<Foo>( new Foo ));
    ASSERT( x );
    ASSERT( !(x == 0) );
  }
  std::cout << "testing rc_ptr constructors from std::unique_ptr..." << std::endl;
  {
    carto::rc_ptr<Bar> x(std::unique_ptr<Bar>( new Bar ));
    ASSERT( x );
    ASSERT( !(x == 0) );
  }

  std::cout << "testing rc_ptr constructors (RCObject)..." << std::endl;
  {
    carto::rc_ptr<Foo> x1;
    ASSERT( x1 == 0 );
    {
      carto::rc_ptr<Foo> x2 = carto::rc_ptr<Foo>( new Foo );
      x1 = x2;
      ASSERT( x1 == x2 );
    }
    ASSERT( x1 != 0 );
  }
  std::cout << "testing rc_ptr constructors..." << std::endl;
  {
    carto::rc_ptr<Bar> x1;
    ASSERT( x1 == 0 );
    {
      carto::rc_ptr<Bar> x2 = carto::rc_ptr<Bar>( new Bar );
      x1 = x2;
      ASSERT( x1 == x2 );
    }
    ASSERT( x1 != 0 );
  }

  std::cout << "testing rc_ptr::reset() (RCObject)..." << std::endl;
  {
    carto::rc_ptr<Foo> x1;
    ASSERT( x1 == 0 );
    x1.reset( new Foo );
    ASSERT( x1 != 0 );
    carto::rc_ptr<Foo> x2( x1 );
    ASSERT( x2 != 0 );
    ASSERT( x1 == x2 );
    x1.reset();
    ASSERT( x1 == 0 );
    ASSERT( x2 != 0 );
    x2.reset();
    ASSERT( x2 == 0 );
  }
  std::cout << "testing rc_ptr::reset()..." << std::endl;
  {
    carto::rc_ptr<Bar> x1;
    ASSERT( x1 == 0 );
    x1.reset( new Bar );
    ASSERT( x1 != 0 );
    carto::rc_ptr<Bar> x2( x1 );
    ASSERT( x2 != 0 );
    ASSERT( x1 == x2 );
    carto::rc_ptr<Bar> x3( x1 );
    ASSERT( x3 != 0 );
    ASSERT( x1 == x3 );
    x1.reset();
    ASSERT( x1 == 0 );
    ASSERT( x2 != 0 );
    x2.reset();
    ASSERT( x2 == 0 );
  }

  std::cout << "testing rc_ptr as a return value (RCObject)..." << std::endl;
  {
    carto::rc_ptr<Foo> x1;
    ASSERT( x1 == 0 );
    x1 = function1();
    ASSERT( x1 != 0 );
  }

  std::cout << "testing rc_ptr as an argument (RCObject)..." << std::endl;
  {
    carto::rc_ptr< Foo > x1( new Foo );
    ASSERT( x1 != 0 );
    carto::rc_ptr< Foo > x2( function2( x1 ) );
    ASSERT( x1 == x2 );
    carto::rc_ptr< Foo > x3( function3( x1 ) );
    ASSERT( x1 == x3 );
    carto::rc_ptr< Foo > x4( function2( function2( x1 ) ) );
    ASSERT( x1 == x4 );
  }

}
