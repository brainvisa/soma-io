#ifndef CARTOBASE_BLOCK_BLOCK_H
#define CARTOBASE_BLOCK_BLOCK_H


#include <cartobase/config/cartobase_config.h>
#include <iterator>
#include <assert.h>


namespace carto
{

/** 	This array class extends the STL.
	It is defined in 
	http://www2.awl.com/cseng/titles/0-201-30956-4/ Generic 
        Programming and the STL,
	pp. 60-67.
	See also http://www2.awl.com/cseng/titles/0-201-88954-4/ 
        The C++ Programming Language,
	3rd edition, pp. 496-497, for a similar class named \p c_array.

        This container class works like a std::vector (STL) but with a fixed 
        size in order to provide a fast and efficient allocation. So it has 
        iterators and begin() / end() methods, but no push_back() / insert() 
        functions.
*/

template < class T, size_t N >
struct block
{

  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;

  typedef ptrdiff_t difference_type;
  typedef size_t size_type;

  typedef pointer iterator;
  typedef const_pointer const_iterator;

#if defined(CARTO_NO_PARTIAL_TEMPLATE_SPECIALIZATION)

  typedef std::reverse_iterator< iterator, value_type > reverse_iterator;
  typedef std::reverse_iterator< const_iterator,
                                 value_type > const_reverse_iterator;
#else

  typedef std::reverse_iterator< iterator > reverse_iterator;
  typedef std::reverse_iterator< const_iterator > const_reverse_iterator;

#endif

  reference operator[]( size_t d );
  const_reference operator[]( size_t d ) const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  reverse_iterator rbegin();
  reverse_iterator rend();
  const_reverse_iterator rbegin() const;
  const_reverse_iterator rend() const;

  size_type size() const;
  size_type max_size() const;
  bool empty() const;

  T data[ N ];

};


} // namespace carto


template < class T, size_t  N>
inline
typename carto::block< T, N >::reference 
carto::block< T, N >::operator[]( size_t d )
{

  assert( d < N );
  return data[ d ];

}


template < class T, size_t N >
inline
typename carto::block< T, N >::const_reference 
carto::block< T, N >::operator[]( size_t d ) const
{

  assert( d < N );
  return data[ d ];

}


template < class T, size_t N >
inline
typename carto::block< T, N >::iterator carto::block< T, N >::begin()
{

  return data;

}


template < class T, size_t N >
inline
typename carto::block< T, N >::iterator carto::block< T, N >::end()
{

  return data + N;

}


template < class T, size_t N >
inline
typename carto::block< T, N >::const_iterator 
carto::block< T, N >::begin() const
{

  return data;

}


template < class T, size_t N >
inline
typename carto::block< T, N >::const_iterator carto::block< T, N >::end() const
{

  return data + N;

}


template < class T, size_t N >
inline
typename carto::block< T, N >::reverse_iterator carto::block< T, N >::rbegin()
{

  return reverse_iterator( end() );

}


template < class T, size_t N >
inline
typename carto::block< T, N >::reverse_iterator carto::block< T, N >::rend()
{

  return reverse_iterator( begin() );

}


template < class T, size_t N >
inline
typename carto::block< T, N >::const_reverse_iterator 
carto::block< T, N >::rbegin() const
{

  return const_reverse_iterator( end() );

}


template < class T, size_t N >
inline
typename carto::block< T, N >::const_reverse_iterator 
carto::block< T, N >::rend() const
{

  return const_reverse_iterator( begin() );

}


template < class T, size_t N >
inline
typename carto::block< T, N >::size_type carto::block< T, N >::size() const
{

  return N;

}


template < class T, size_t N >
inline
typename carto::block< T, N >::size_type carto::block< T, N >::max_size() const
{

  return N;

}


template < class T, size_t N >
inline
bool carto::block< T, N >::empty() const
{

  return N == 0;

}


template < class T, size_t N >
inline
bool operator==( const carto::block< T, N >& b1,
                 const carto::block< T, N >& b2)
{

  for ( size_t n = 0; n < N; ++n )
    if ( b1.data[ n ] != b2.data[ n ] )
      return false;
  return true;

}


#endif
