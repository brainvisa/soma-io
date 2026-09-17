
#ifndef CARTOBASE_SMART_RCPTRTRICK_H
#define CARTOBASE_SMART_RCPTRTRICK_H

#include <cartobase/smart/rcptr.h>
#include <cartobase/smart/sharedptr.h>

namespace carto
{

  /** rc_ptr_trick merely makes a public access to the reference counter 
      inside a rc_ptr. It is intended to be used only on low-level, hardly 
      legal dirty operations, as its name tells. It is *not* intended to be 
      used by normal programs, so just DON'T USE IT.
   */
  class rc_ptr_trick
  {
  public:
    /** just increments the ref-counter of the underlying rc_ptr so that 
        the rc_ptr will never delete the object in it (it doesn't own 
        the object anymore) */
    template <typename T> 
    static void releaseOwnership( rc_ptr<T> & );
    static void releaseOwnership( RCObject & );
    /// decrements the ref-counter
    template <typename T> 
    static void restoreOwnership( rc_ptr<T> & );
    static void restoreOwnership( RCObject & );
    /// allows complete read-write access to the counter
    template <typename T>
    static RCObject::RefCounterType & refCount( shared_ptr<T> & );
    template <typename T>
    static RCObject::RefCounterType & refCount( rc_ptr<T> & );
    static RCObject::RefCounterType & refCount( RCObject & );
    template <typename T>
    static RCObject::RefCounterType & weakCount( weak_shared_ptr<T> & );
    static RCObject::RefCounterType & weakCount( SharedObject & );
  };


  template <typename T> inline
  void rc_ptr_trick::releaseOwnership( rc_ptr<T> & x )
  {
    if( x._pcount && *x._pcount > 0 )
      ++(*x._pcount);
  }


  inline void rc_ptr_trick::releaseOwnership( RCObject & x )
  {
    if( x._refCounter > 0 )
      ++x._refCounter;
  }


  template <typename T> inline
  void rc_ptr_trick::restoreOwnership( rc_ptr<T> & x )
  {
    if( x._pcount && *x._pcount > 0 )
      --(*x._pcount);
  }


  inline void rc_ptr_trick::restoreOwnership( RCObject & x )
  {
    if( x._refCounter > 0 )
      --x._refCounter;
  }


  template <typename T> inline
  RCObject::RefCounterType & rc_ptr_trick::refCount( shared_ptr<T> & x )
  {
    return x->_refCounter;
  }


  template <typename T> inline
  RCObject::RefCounterType & rc_ptr_trick::refCount( rc_ptr<T> & x )
  {
    return *x._pcount;
  }


  inline RCObject::RefCounterType & rc_ptr_trick::refCount( RCObject & x )
  {
    return x._refCounter;
  }


  template <typename T> inline
  RCObject::RefCounterType &
  rc_ptr_trick::weakCount( weak_shared_ptr<T> & x )
  {
    return x->weak_count;
  }


  inline RCObject::RefCounterType &
  rc_ptr_trick::weakCount( SharedObject & x )
  {
    return x.weak_count;
  }

}

#endif

