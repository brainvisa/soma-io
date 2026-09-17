#include <cartobase/smart/weakobject.h>
#include <cartobase/smart/sharedptr.h>


using carto::WeakObject;


/*!
  \class carto::WeakObject weakobject.h "cartograph-base/rcptr/weakobject.h"
  \brief Base class for weakly referenced objects

  WeakObject implements a specialized observer/subject pattern where watch_ptr
  is the observer.

  \sa rc_ptr weak_ptr
*/


/*!
  ...
*/
WeakObject::WeakObject()
{
}


/*!
  A weak object is copy-constructed.  It's a new object so it has no observers.
*/
WeakObject::WeakObject( const WeakObject& )
{
}


/*!
  A weak object is copied.  It's a new object so it has no observers.
*/
WeakObject& WeakObject::operator=( const WeakObject& )
{
  return *this;
}


/*!
  Notify observing watch_ptr%s upon destruction.
*/
WeakObject::~WeakObject()
{
  notifyObervers();
}


/*!
  Notify observing watch_ptr%s.
*/
void WeakObject::notifyObervers() const
{
  std::map< WeakPtr*, bool >::const_iterator i = observers.begin();
  std::map< WeakPtr*, bool >::const_iterator e = observers.end();
  while ( i != e )
  {
    i->first->update();
    if( i->second )
      (static_cast<weak_shared_ptr<WeakObject> *>(i->first))->update();
    ++i;
  }
}


/*!
  Avoid expanding STL templates inline.
*/
void WeakObject::attachWeakPtr( WeakPtr* ptr ) const
{
  observers[ ptr ] = false;
}


/*!
  Avoid expanding STL templates inline.
*/
void WeakObject::detachWeakPtr( WeakPtr* ptr ) const
{
  observers.erase( ptr );
}


/*!
  Avoid expanding STL templates inline.
 */
void WeakObject::attachWeakPtr( WeakSharedPtr* ptr ) const
{
  observers[ ptr ] = true;
}


/*!
  Avoid expanding STL templates inline.
 */
void WeakObject::detachWeakPtr( WeakSharedPtr* ptr ) const
{
  observers.erase( ptr );
}
