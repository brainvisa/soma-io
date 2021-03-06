/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

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
