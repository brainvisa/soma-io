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

#ifndef SOMAIO_UTILITIES_ALLOCATEDVECTOR_H
#define SOMAIO_UTILITIES_ALLOCATEDVECTOR_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/allocator/allocator.h>
#include <soma-io/datasource/bufferdatasource.h>
//----------------------------------------------------------------------------

namespace soma
{

  /// A STL-like vector with a smart and slow allocation system for large
  /// data buffers.
  ///
  /// AllocatedVector is a fixed-size vector (no push_back(), insert()
  /// functions, nor reallocation method) which is suitable for very large
  /// data that may need the use of memory mapping and/or partial input/output
  /// functionalities. However for small data blocks, this container class
  /// is not optimal, because the allocation system is rather slow when
  /// repeatedly used many times.
  ///
  /// Otherwise, AllocatedVector works like a std::vector, or rather like a
  /// \link carto::block block\endlink.
  ///
  /// The allocation system uses an allocation context (AllocatorContext) to
  /// automatically switch to the most suitable type of allocation (or memory
  /// mapping). This system is describe in the \ref allocators section.
  template <typename T>
  class AllocatedVector
  {
    public:
      typedef size_t size_type;
      typedef T* iterator;
      typedef const T* const_iterator;

      AllocatedVector( size_type n, const AllocatorContext & ac );
      /// The copy constructor uses a "standard" AllocatorContext using memory
      /// allocation.
      AllocatedVector( const AllocatedVector & other );
      /// This copy constructor variant allows to specify a custom
      /// AllocatorContext
      AllocatedVector( const AllocatedVector & other,
                       const AllocatorContext & ac );
      /// This constructor builds a vector on an already allocated buffer.
      /// The vector is not owner of the underlying data.
      AllocatedVector( size_type n, T* buffer );
      ~AllocatedVector();

      size_type size() const;
      iterator begin();
      const_iterator begin() const;
      iterator end();
      const_iterator end() const;
      T & operator [] ( size_type i );
      const T & operator [] ( size_type i ) const;
      /// The copy operator uses a "standard" AllocatorContext using memory
      /// allocation.
      AllocatedVector & operator = ( const AllocatedVector & );

      const AllocatorContext & allocatorContext() const;

      void free();
      void allocate( size_type n, const AllocatorContext & ac );

    private:
      AllocatorContext _allocatorContext;
      size_type _size;
      T *_items;
  };

  //==========================================================================
  //   I N L I N E   F U N C T I O N S
  //==========================================================================

  template<typename T> inline
  AllocatedVector<T>::AllocatedVector( size_type n,
                                       const AllocatorContext &ac )
    : _allocatorContext( ac ), _size( n ),
      _items( _allocatorContext.allocate( _items, n ) )
  {
  }

  template<typename T> inline
  AllocatedVector<T>::AllocatedVector( const AllocatedVector & other )
    : _allocatorContext(), _size( other.size() ),
      _items( _allocatorContext.allocate( _items, _size ) )
  {
    if( _allocatorContext.allocatorType() != AllocatorStrategy::ReadOnlyMap
        && _allocatorContext.isAllocated()
        && other._allocatorContext.isAllocated() )
      {
        iterator i, e = end();
        const_iterator j = other.begin();
        for( i = begin(); i != e; ++i, ++j )
          *i = *j;
      }
  }

  template<typename T> inline
  AllocatedVector<T>::AllocatedVector( const AllocatedVector & other,
                                       const AllocatorContext &ac )
    : _allocatorContext( ac ), _size( other.size() ),
      _items( _allocatorContext.allocate( _items, _size ) )
  {
    if( _allocatorContext.allocatorType() != AllocatorStrategy::ReadOnlyMap
        && _allocatorContext.isAllocated()
        && other._allocatorContext.isAllocated() )
      {
        iterator i, e = end();
        const_iterator j = other.begin();
        for( i = begin(); i != e; ++i, ++j )
          *i = *j;
      }
  }

  template<typename T> inline
  AllocatedVector<T>::AllocatedVector( size_type n, T* buffer )
    : _allocatorContext( AllocatorStrategy::NotOwner,
                         carto::rc_ptr<DataSource>
                         ( new BufferDataSource( reinterpret_cast<char *>
                                                 ( buffer ), n * sizeof( T ),
                                                 DataSource::ReadWrite ) ) ),
      _size( n ),
      _items( _allocatorContext.allocate( _items, _size ) )
  {
  }

  template<typename T> inline
  AllocatedVector<T>::~AllocatedVector()
  {
    free();
  }

  template<typename T> inline
  const AllocatorContext & AllocatedVector<T>::allocatorContext() const
  {
    return _allocatorContext;
  }

  template<typename T> inline
  void AllocatedVector<T>::allocate( size_type n,
                                     const AllocatorContext & ac )
  {
    free();
    _size = n;
    _allocatorContext = ac;
    _items = _allocatorContext.allocate( _items, n );
  }

  template<typename T> inline
  void AllocatedVector<T>::free()
  {
    _allocatorContext.deallocate( _items, _size );
    _size = 0;
    _items = 0;
  }

  template<typename T> inline
  typename AllocatedVector<T>::size_type AllocatedVector<T>::size() const
  {
    return _size;
  }

  template<typename T> inline
  AllocatedVector<T> &
  AllocatedVector<T>::operator = ( const AllocatedVector & other )
  {
    if( &other == this )
      return *this;
    allocate( other.size(), AllocatorContext() );
    if( _allocatorContext.allocatorType() != AllocatorStrategy::ReadOnlyMap
        && _allocatorContext.isAllocated()
        && other._allocatorContext.isAllocated() )
      {
        iterator i, e = end();
        const_iterator j = other.begin();
        for( i = begin(); i != e; ++i, ++j )
          *i = *j;
      }
    return *this;
  }

  template<typename T> inline
  typename AllocatedVector<T>::iterator AllocatedVector<T>::begin()
  {
    return _items;
  }

  template<typename T> inline
  typename AllocatedVector<T>::const_iterator
  AllocatedVector<T>::begin() const
  {
    return _items;
  }

  template<typename T> inline
  typename AllocatedVector<T>::iterator AllocatedVector<T>::end()
  {
    return _items + _size;
  }

  template<typename T> inline
  typename AllocatedVector<T>::const_iterator AllocatedVector<T>::end() const
  {
    return _items + _size;
  }

  template<typename T> inline
  T & AllocatedVector<T>::operator [] ( size_type i )
  {
    return _items[i];
  }

  template<typename T> inline
  const T & AllocatedVector<T>::operator [] ( size_type i ) const
  {
    return _items[i];
  }

}

#endif
