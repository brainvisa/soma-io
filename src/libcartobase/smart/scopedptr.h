//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

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

#ifndef CARTOBASE_SMART_SCOPEDPTR_H
#define CARTOBASE_SMART_SCOPEDPTR_H


#include <cartobase/config/cartobase_config.h>
#include <memory>


namespace carto {


template <typename T>
class scoped_ptr
{
  public:

    typedef T element_type;

#ifndef DOXYGEN_HIDE_INTERNAL_CLASSES
    class InsipidProxyPointer
    {
      protected:
        void operator delete( void* );
        void operator delete[]( void* );
    };
#endif

    explicit scoped_ptr( T* p = 0 ) : pointee( p )
    {
    }

#if __cplusplus >= 201103L
    // Only unique_ptr with the default deleter can be converted, because
    // scoped_ptr does not support custom deleters (it calls the delete operator
    // upon destruction, just like std::default_delete).
    template< class U >
    scoped_ptr( std::unique_ptr<U>&& r ) : pointee( r.release() )
    {
    }
#else
    template< class U >
    scoped_ptr( std::auto_ptr<U> r ) : pointee( r.release() )
    {
    }
#endif

    ~scoped_ptr()
    {
      delete pointee;
    }

    void reset( T* p = 0 )
    {
      if ( pointee != p )
      {
        delete pointee;
        pointee = p;
      }
    }

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

#ifndef DOXYGEN_HIDE_INTERNAL_CLASSES
    operator InsipidProxyPointer*() const
    {
      return reinterpret_cast<InsipidProxyPointer*>( pointee );
    }
#endif

    void swap( scoped_ptr & r )
    {
      T* tmp( r.pointee );
      r.pointee = pointee;
      pointee = tmp;
    }

  private:

    scoped_ptr( const scoped_ptr& )
#if __cplusplus >= 201103L
    = delete
#endif
      ;
    const scoped_ptr& operator=( const scoped_ptr& )
#if __cplusplus >= 201103L
    = delete
#endif
      ;

    T* pointee;

};


template <typename T>
inline
void swap( scoped_ptr<T>& x, scoped_ptr<T>& y )
{
  x.swap(y);
}


} // namespace carto


#endif
