#include <cartobase/smart/sharedptr.h>
#include <cartobase/smart/rcptrtrick.h>

using namespace carto;


SharedObject::SharedObject()
  : WeakObject(), RCObject(), weak_count( 0 )
{
}


SharedObject::SharedObject( const SharedObject & x )
  : WeakObject( x ), RCObject( x ), weak_count( 0 )
{
}


SharedObject::~SharedObject()
{
  disableRefCount();
  // notifyUnregisterObservers(); // done in WeakObject
}


bool SharedObject::testDeletable()
{
  return weak_count != 0 && weak_count == rc_ptr_trick::refCount( *this );
}


bool SharedObject::tryDelete()
{
  if( testDeletable() )
  {
    // force the counter to -1 before deleting to prevent multiple deletion
    disableRefCount();
    delete this;
    return true;
  }
  return false;
}


void SharedObject::disableRefCount()
{
#if defined( CARTO_RCPTR_USE_BOOST )
  // boost doesn't allow assignation of atomic counters
  while( rc_ptr_trick::refCount( *this ) >= 0 )
    --rc_ptr_trick::refCount( *this );
#else
  rc_ptr_trick::refCount( *this ) = -1;
#endif
}

