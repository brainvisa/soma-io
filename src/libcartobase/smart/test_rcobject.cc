
#include <cartobase/smart/test_rcobject.h>
#include <iostream>


using namespace carto;
using namespace std;


unsigned long TestRCObject::global_count = 0;


TestRCObject::TestRCObject( bool verbose )
  : RCObject(), _verbose( verbose )
{
  ++global_count;
  if( _verbose )
    cout << "TestRCObject::TestRCObject " << this << ", global count: "
      << global_count << endl;
}


TestRCObject::~TestRCObject()
{
  --global_count;
  if( _verbose )
    cout << "TestRCObject::~TestRCObject " << this << ", global count: "
      << global_count << endl;
}


void TestRCObject::setVerbose( bool verbose )
{
  _verbose = verbose;
}


unsigned long TestRCObject::globalCount()
{
  return global_count;
}


RCObject* TestRCObject::newTestObject()
{
  return new TestRCObject;
}

