////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef CARTOBASE_SMART_WEAKPTR_H
#define CARTOBASE_SMART_WEAKPTR_H


#include <cartobase/config/cartobase_config.h>
#include <cartobase/smart/rcptr.h>
#include <memory>


namespace carto {


template <typename T> class weak_ptr;
template <typename T, typename U> weak_ptr<U> weak_cast( const weak_ptr<T>& );
template <typename T> class weak_shared_ptr;
template <typename T> class shared_ptr;


template <typename T>
class weak_ptr
{

    friend class WeakObject;

#if !defined( CARTO_NO_MEMBER_TEMPLATES ) && \
    !defined( CARTO_NO_MEMBER_TEMPLATE_FRIENDS )
    template <typename Y> friend class weak_ptr;
#endif

#if defined( CARTO_BROKEN_FRIEND_TEMPLATE_FUNCTION )
    template <typename U, typename V>
      friend weak_cast( const weak_ptr< U >& );
#else
    template < typename U, typename V >
      friend weak_ptr< V > weak_cast( const weak_ptr< U >& );
#endif

#ifndef DOXYGEN_HIDE_INTERNAL_CLASSES
    class InsipidProxyPointer
    {
      protected:
        void operator delete( void* );
        void operator delete[]( void* );
    };
#endif

  public:

    explicit weak_ptr( T* p = 0 ) : pointee( p )
    {
      if ( pointee )
        pointee->attachWeakPtr( *this );
    }

    void reset( T* r = 0 )
    {
      if ( pointee != r )
      {
        if ( pointee )
        {
          pointee->detachWeakPtr( *this );
          pointee = 0;
        }
        if ( r )
        {
          r->attachWeakPtr( *this );
        }
        pointee = r;
      }
    }

#if !defined( CARTO_NO_MEMBER_TEMPLATES )

    template <typename Y>
    weak_ptr( const weak_ptr<Y>& r ) : pointee( r.pointee )
    {
      if ( pointee )
        pointee->attachWeakPtr( *this );
    }

    template <typename Y>
    explicit weak_ptr( const rc_ptr<Y>& r ) : pointee( r.get() )
    {
      if ( pointee )
        pointee->attachWeakPtr( *this );
    }

    template <typename Y>
    explicit weak_ptr( const std::unique_ptr<Y>& r ) : pointee( r.get() )
    {
      if ( pointee )
        pointee->attachWeakPtr( *this );
    }

    template <typename Y>
    weak_ptr& operator=( const weak_ptr<Y>& r )
    {
      reset( r.get() );
      return *this;
    }

    template <typename Y>
    weak_ptr& operator=( const rc_ptr<Y>& r )
    {
      reset( r.get() );
      return *this;
    }

    template <typename Y>
    weak_ptr& operator=( const std::unique_ptr<Y>& r )
    {
      reset( r.get() );
      return *this;
    }

#else

    explicit weak_ptr( const rc_ptr<T>& r ) : pointee( r.get() )
    {
      if ( pointee )
        pointee->attachWeakPtr( *this );
    }

    explicit weak_ptr( const std::unique_ptr<T>& r ) : pointee( r.get() )
    {
      if ( pointee )
        pointee->attachWeakPtr( *this );
    }

    weak_ptr& operator=( const rc_ptr<T>& r )
    {
      reset( r.get() );
      return *this;
    }

    weak_ptr& operator=( const std::unique_ptr<T>& r )
    {
      reset( r.get() );
      return *this;
    }

#endif

    weak_ptr( const weak_ptr& r ) : pointee( r.pointee )
    {
      if ( pointee )
        pointee->attachWeakPtr( *this );
    }

    weak_ptr& operator=( const weak_ptr& w )
    {
      reset( w.get() );
      return *this;
    }

    ~weak_ptr()
    {
      if ( pointee )
        pointee->detachWeakPtr( *this );
    }

#ifndef DOXYGEN_HIDE_INTERNAL_CLASSES
    operator InsipidProxyPointer*() const
    {
      return reinterpret_cast<InsipidProxyPointer*>( pointee );
    }
#endif

    T& operator*() const
    {
      return *pointee;
    }

    T* operator->() const 
    {
      return pointee;
    }

    T* get() const
    {
      return pointee;
    }

    T* release()
    {
      if ( pointee )
        pointee->detachWeakPtr( *this );
      T* tmp = pointee;
      pointee = 0;
      return tmp;
    }

    bool operator == ( const weak_ptr<T> & x ) const
    { return pointee == x.get(); }
    bool operator == ( const weak_shared_ptr<T> & x ) const
    { return pointee == x.get(); }
    bool operator == ( const rc_ptr<T> & x ) const
    { return pointee == x.get(); }
    bool operator == ( const shared_ptr<T> & x ) const
    { return pointee == x.get(); }
    bool operator != ( const weak_ptr<T> & x ) const
    { return pointee != x.get(); }
    bool operator != ( const weak_shared_ptr<T> & x ) const
    { return pointee != x.get(); }
    bool operator != ( const rc_ptr<T> & x ) const
    { return pointee != x.get(); }
    bool operator != ( const shared_ptr<T> & x ) const
    { return pointee != x.get(); }
    bool operator < ( const weak_ptr<T> & x ) const
    { return pointee < x.get(); }
    bool operator < ( const weak_shared_ptr<T> & x ) const
    { return pointee < x.get(); }
    bool operator < ( const shared_ptr<T> & x ) const
    { return pointee < x.get(); }
    bool operator < ( const rc_ptr<T> & x ) const
    { return pointee < x.get(); }

  private:

    void update()
    {
      pointee = 0;
    }

#if defined( CARTO_NO_MEMBER_TEMPLATES) || \
    !defined( CARTO_NO_MEMBER_TEMPLATE_FRIENDS )
  private:
#else
  public:
#endif

    T* pointee;

};


} // namespace carto


template <typename T, typename U>
inline
carto::weak_ptr<U> carto::weak_cast( const carto::weak_ptr<T>& r )
{
  return weak_ptr<U>( dynamic_cast<U*>( r.pointee ) );
}


#endif
