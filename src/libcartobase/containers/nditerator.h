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

#ifndef CARTOBASE_CONTAINER_NDITERATOR_H
#define CARTOBASE_CONTAINER_NDITERATOR_H

#include <cartobase/config/cartobase_config.h>
#include <vector>
#include <iostream>


namespace carto
{

  /** Base class for N-dimensional array iterators.

      See NDIterator, const_NDIterator, line_NDIterator, and
      const_line_NDIterator.
  */
  class NDIterator_base
  {
  public:
    NDIterator_base( const std::vector<int> & dims );
    NDIterator_base( const std::vector<int> & dims,
                     const std::vector<int> & strides );
    NDIterator_base( const std::vector<int> & dims,
                     const std::vector<size_t> & strides );
    NDIterator_base( const std::vector<int> & dims,
                     const std::vector<long> & strides );
    // virtual ~NDIterator_base() {}
    const std::vector<int> & position() const;
    uint64_t offset() const;

    NDIterator_base & operator ++();

    bool ended() const;
    void reset();

  protected:
    template <typename T>
    static std::vector<size_t> to_int_v( const std::vector<T> & v );

    std::vector<int> _dims;
    std::vector<size_t> _strides;
    std::vector<int> _position;
    uint64_t _offset;
    bool _ended;
  };


  class line_NDIterator_base : public NDIterator_base
  {
  public:
    line_NDIterator_base( const std::vector<int> & dims );
    line_NDIterator_base( const std::vector<int> & dims,
                          const std::vector<int> & strides,
                          bool optimize_direction = false );
    line_NDIterator_base( const std::vector<int> & dims,
                          const std::vector<size_t> & strides,
                          bool optimize_direction = false );
    line_NDIterator_base( const std::vector<int> & dims,
                          const std::vector<long> & strides,
                          bool optimize_direction = false );
    // virtual ~line_NDIterator_base() {}

    NDIterator_base & operator ++();

    int line_size() const;
    int line_direction() const;
    bool is_contiguous() const;

  protected:
    void set_optimized_direction( bool optimize_direction = false );

    int _line_directon;
    long _line_stride;
    bool _contiguous;
  };


  /** N-dimensional array iterator

      Iterates over every element of a N-D array, using strides. The iterator
      is used more or less like a pointer:

      \code
      float *data; // fill it...
      std::vector<int> dimensions;
      std::vector<int> strdes;
      // fill dimensions and strides...
      float sum = 0.;
      NDIterator<float> it( data, dimensions, strides );
      for( ; !it.ended(); ++it )
        sum += *it;
      \endcode

      Note that the NDIterator is somewhat sub-optimal since it has to test,
      at each increment, the bounds of each dimension. In many cases it is
      possible to lowerage the overhead by implementing an ad-hoc loop for the
      smallest dimension, and using a line_NDIterator on higher dimensions.

  */
  template <typename T> class NDIterator : public NDIterator_base
  {
  public:
    NDIterator( T* buffer, const std::vector<int> & dims );
    NDIterator( T* buffer, const std::vector<int> & dims,
                const std::vector<int> & strides );
    NDIterator( T*buffer, const std::vector<int> & dims,
                const std::vector<size_t> & strides );
    NDIterator( T* buffer, const std::vector<int> & dims,
                const std::vector<long> & strides );
    // virtual ~NDIterator() {}

    T & operator * () const;

  protected:
    T* _buffer;
  };


  /// const variant of the NDIterator
  template <typename T> class const_NDIterator : public NDIterator_base
  {
  public:
    const_NDIterator( const T* buffer, const std::vector<int> & dims );
    const_NDIterator( const T* buffer, const std::vector<int> & dims,
                      const std::vector<int> & strides );
    const_NDIterator( const T* buffer, const std::vector<int> & dims,
                      const std::vector<size_t> & strides );
    const_NDIterator( const T* buffer, const std::vector<int> & dims,
                      const std::vector<long> & strides );
    // virtual ~const_NDIterator() {}

    const T & operator * () const;

  protected:
    const T* _buffer;
  };


  /** N-dimensional array line iterator

      Iterates over every "line" of a N-D array, using strides. The "lines"
      are arrays over the smallest dimension of the array. Thus the iterator
      iterates over all dimensions but the smallest one. This allows to
      optimize code iterating directly on the smallest dimension, which is
      much more efficient than the regular NDIterator: for a very simple item
      operation using a line_NDIterator instead of a NDIterator can be about
      20 times faster.

      \code
      float *data; // fill it...
      std::vector<int> dimensions;
      std::vector<int> strdes;
      // fill dimensions and strides...
      float sum = 0.;
      line_NDIterator<float> it( data, dimensions, strides );
      float *p, *pp;
      for( ; !it.ended(); ++it )
      {
        p = &*it;
        for( pp=p + it.line_length(); p!=pp; it.inc_line_ptr( p ) )
          sum += *p;
      }
      \endcode
  */
  template <typename T> class line_NDIterator : public line_NDIterator_base
  {
  public:
    line_NDIterator( T* buffer, const std::vector<int> & dims );
    /** the optional "optimize_direction" parameter allows the iterator to
        find a contiguous direction (with a stride == 1) and use it as the line
        dimension. This way a line pointer can be incremented instead of adding
        strides. In many cases it does not change much in performance.
    */
    line_NDIterator( T* buffer, const std::vector<int> & dims,
                     const std::vector<int> & strides,
                     bool optimize_direction = false );
    line_NDIterator( T* buffer, const std::vector<int> & dims,
                     const std::vector<size_t> & strides,
                     bool optimize_direction = false );
    line_NDIterator( T* buffer, const std::vector<int> & dims,
                     const std::vector<long> & strides,
                     bool optimize_direction = false );
    // virtual ~line_NDIterator() {}

    T & operator * () const;
    /// increment a "line" pointer (advancing through the line)
    void inc_line_ptr( T* & p ) const;
    /// increment a "line" pointer (advancing through the line)
    void inc_line_ptr( const T* & p ) const;
    /// line_size * line stride: end of line
    long line_length() const;

  protected:
    T* _buffer;
  };


  /// const variant of the line_NDIterator
  template <typename T> class const_line_NDIterator
    : public line_NDIterator_base
  {
  public:
    const_line_NDIterator( const T* buffer, const std::vector<int> & dims );
    /** the optional "optimize_direction" parameter allows the iterator to
        find a contiguous direction (with a stride == 1) and use it as the line
        dimension. This way a line pointer can be incremented instead of adding
        strides. In many cases it does not change much in performance.
    */
    const_line_NDIterator( const T* buffer, const std::vector<int> & dims,
                           const std::vector<int> & strides,
                           bool optimize_direction = false );
    const_line_NDIterator( const T* buffer, const std::vector<int> & dims,
                           const std::vector<size_t> & strides,
                           bool optimize_direction = false );
    const_line_NDIterator( const T* buffer, const std::vector<int> & dims,
                           const std::vector<long> & strides,
                           bool optimize_direction = false );
    // virtual ~const_line_NDIterator() {}

    const T & operator * () const;
    /// increment a "line" pointer (advancing through the line)
    void inc_line_ptr( const T* & p ) const;
    /// line_size * line stride: end of line
    long line_length() const;

  protected:
    const T* _buffer;
  };


  // -- implementation --

  inline NDIterator_base::NDIterator_base( const std::vector<int> & dims )
    : _dims( dims ), _position( dims.size(), 0 ), _offset( 0 ), _ended( false )
  {
    if( dims.empty() )
      _ended = true;
  }


  inline NDIterator_base::NDIterator_base( const std::vector<int> & dims,
    const std::vector<int> & strides )
    : _dims( dims ), _strides( NDIterator_base::to_int_v( strides ) ),
      _position( dims.size(), 0 ),
      _offset( 0 ), _ended( false )
  {
    if( dims.empty() )
      _ended = true;
  }


  inline NDIterator_base::NDIterator_base( const std::vector<int> & dims,
    const std::vector<size_t> & strides )
    : _dims( dims ), _strides( strides ), _position( dims.size(), 0 ),
      _offset( 0 ), _ended( false )
  {
    if( dims.empty() )
      _ended = true;
  }


  inline NDIterator_base::NDIterator_base( const std::vector<int> & dims,
    const std::vector<long> & strides )
    : _dims( dims ), _strides( NDIterator_base::to_int_v( strides ) ),
      _position( dims.size(), 0 ),
      _offset( 0 ), _ended( false )
  {
    if( dims.empty() )
      _ended = true;
  }


  inline const std::vector<int> & NDIterator_base::position() const
  {
    return _position;
  }


  inline uint64_t NDIterator_base::offset() const
  {
    return _offset;
  }


  inline bool NDIterator_base::ended() const
  {
    return _ended;
  }


  inline void NDIterator_base::reset()
  {
    _offset = 0;
    _position = std::vector<int>( _dims.size(), 0 );
  }


  template <typename T> inline
  std::vector<size_t> NDIterator_base::to_int_v( const std::vector<T> & v )
  {
    std::vector<size_t> ov( v.size() );
    std::vector<size_t>::iterator io = ov.begin();
    typename std::vector<T>::const_iterator i, e = v.end();
    for( i=v.begin(); i!=e; ++i, ++io )
      *io = (size_t) *i;
    return ov;
  }


  inline NDIterator_base & NDIterator_base::operator ++()
  {
    size_t dim, ndim = _dims.size();
    bool nextrow = true, stride = !_strides.empty();
    for( dim=0; nextrow && dim<ndim; ++dim )
    {
      if( nextrow )
      {
        ++_position[dim];
        if( stride )
          _offset += _strides[dim];
        if( _position[dim] == _dims[dim] )
        {
          if( dim == ndim - 1 )
            _ended = true;
          _position[dim] = 0;
          if( stride )
            _offset -= _strides[dim] * _dims[dim];
        }
        else
          nextrow = false;
      }
    }
    return *this;
  }

  // --

  inline line_NDIterator_base::line_NDIterator_base(
    const std::vector<int> & dims )
    : NDIterator_base( dims ), _line_directon( 0 ), _line_stride( 1 ),
      _contiguous( true )
  {
    if( dims.size() < 2 )
      _ended = true;
  }


  inline line_NDIterator_base::line_NDIterator_base(
    const std::vector<int> & dims, const std::vector<int> & strides,
    bool optimize_direction )
    : NDIterator_base( dims, strides ), _line_directon( 0 ), _line_stride( 1 ),
      _contiguous( true )
  {
    if( dims.size() < 2 )
      _ended = true;
    set_optimized_direction( optimize_direction );
  }


  inline line_NDIterator_base::line_NDIterator_base(
    const std::vector<int> & dims, const std::vector<size_t> & strides,
    bool optimize_direction )
    : NDIterator_base( dims, strides ), _line_directon( 0 ), _line_stride( 1 ),
      _contiguous( true )
  {
    if( dims.size() < 2 )
      _ended = true;
    set_optimized_direction( optimize_direction );
  }


  inline line_NDIterator_base::line_NDIterator_base(
    const std::vector<int> & dims, const std::vector<long> & strides,
    bool optimize_direction )
    : NDIterator_base( dims, strides ), _line_directon( 0 ), _line_stride( 1 ),
      _contiguous( true )
  {
    if( dims.size() < 2 )
      _ended = true;
    set_optimized_direction( optimize_direction );
  }


  inline void line_NDIterator_base::set_optimized_direction(
    bool optimize_direction )
  {
    if( !_strides.empty() )
    {
      _line_stride = _strides[_line_directon];
      if( optimize_direction )
      {
        size_t dim, ndim = _strides.size();
        for( dim=0; dim<ndim; ++dim )
          if( _strides[dim] == 1 )
          {
            // elements are contiguous along this direction
            _line_directon = dim;
            _line_stride = 1;
            break;
          }
      }
    }
    else
      _line_stride = 1;
    _contiguous = ( _strides.empty() || _strides[_line_directon] == 1 );
  }


  inline bool line_NDIterator_base::is_contiguous() const
  {
    return _contiguous;
  }


  inline int line_NDIterator_base::line_direction() const
  {
    return _line_directon;
  }


  inline int line_NDIterator_base::line_size() const
  {
    return _dims[_line_directon];
  }


  inline NDIterator_base & line_NDIterator_base::operator ++()
  {
    size_t dim, ndim = _dims.size();
    bool nextrow = true, stride = !_strides.empty();
    for( dim=0; nextrow && dim<ndim; ++dim )
    {
      if( dim == _line_directon )
        continue;
      ++_position[dim];
      if( stride )
        _offset += _strides[dim];
      if( _position[dim] == _dims[dim] )
      {
        if( dim == ndim - 1
            || ( dim == ndim - 2 && _line_directon == ndim - 1 ) )
          _ended = true;
        _position[dim] = 0;
        if( stride )
          _offset -= _strides[dim] * _dims[dim];
      }
      else
        nextrow = false;
    }
    return *this;
  }

  // --

  template <typename T> inline
  NDIterator<T>::NDIterator( T* buffer, const std::vector<int> & dims )
    : NDIterator_base( dims ), _buffer( buffer )
  {
  }


  template <typename T> inline
  NDIterator<T>::NDIterator( T* buffer, const std::vector<int> & dims,
                             const std::vector<int> & strides )
    : NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> inline
  NDIterator<T>::NDIterator( T* buffer, const std::vector<int> & dims,
                             const std::vector<size_t> & strides )
    : NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> inline
  NDIterator<T>::NDIterator( T* buffer, const std::vector<int> & dims,
                             const std::vector<long> & strides )
    : NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> inline T & NDIterator<T>::operator * () const
  {
    return _buffer[ _offset ];
  }

  // --

  template <typename T>  inline
  const_NDIterator<T>::const_NDIterator(
    const T* buffer, const std::vector<int> & dims )
    : NDIterator_base( dims ), _buffer( buffer )
  {
  }


  template <typename T> inline
  const_NDIterator<T>::const_NDIterator(
    const T* buffer, const std::vector<int> & dims,
    const std::vector<int> & strides )
    : NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> inline
  const_NDIterator<T>::const_NDIterator(
    const T* buffer, const std::vector<int> & dims,
    const std::vector<long> & strides )
    : NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> inline
  const_NDIterator<T>::const_NDIterator(
    const T* buffer, const std::vector<int> & dims,
    const std::vector<size_t> & strides )
    : NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> const inline T &
  const_NDIterator<T>::operator * () const
  {
    return _buffer[ _offset ];
  }

  // --

  template <typename T> inline
  line_NDIterator<T>::line_NDIterator(
    T* buffer, const std::vector<int> & dims )
    : line_NDIterator_base( dims ), _buffer( buffer )
  {
  }


  template <typename T> inline
  line_NDIterator<T>::line_NDIterator( T* buffer,
                                       const std::vector<int> & dims,
                                       const std::vector<int> & strides,
                                       bool optimize_direction )
    : line_NDIterator_base( dims, strides, optimize_direction ),
      _buffer( buffer )
  {
  }


  template <typename T> inline
  line_NDIterator<T>::line_NDIterator( T* buffer,
                                       const std::vector<int> & dims,
                                       const std::vector<size_t> & strides,
                                       bool optimize_direction )
    : line_NDIterator_base( dims, strides, optimize_direction ),
      _buffer( buffer )
  {
  }


  template <typename T> inline
  line_NDIterator<T>::line_NDIterator( T* buffer,
                                       const std::vector<int> & dims,
                                       const std::vector<long> & strides,
                                       bool optimize_direction )
    : line_NDIterator_base( dims, strides, optimize_direction ),
      _buffer( buffer )
  {
  }


  template <typename T> inline T & line_NDIterator<T>::operator * () const
  {
    return _buffer[ _offset ];
  }


  template <typename T> inline
  void line_NDIterator<T>::inc_line_ptr( T* & p ) const
  {
    if( _contiguous )
      ++p;
    else
      p += _line_stride;
  }


  template <typename T> inline
  void line_NDIterator<T>::inc_line_ptr( const T* & p ) const
  {
    if( _contiguous )
      ++p;
    else
      p += _line_stride;
  }


  template <typename T> inline
  long line_NDIterator<T>::line_length() const
  {
    if( is_contiguous() )
      return line_size();
    else
      return line_size() * _strides[_line_directon];
  }

  // --

  template <typename T> inline
  const_line_NDIterator<T>::const_line_NDIterator(
    const T* buffer, const std::vector<int> & dims )
    : line_NDIterator_base( dims ), _buffer( buffer )
  {
  }


  template <typename T> inline
  const_line_NDIterator<T>::const_line_NDIterator(
    const T* buffer, const std::vector<int> & dims,
    const std::vector<int> & strides, bool optimize_direction )
    : line_NDIterator_base( dims, strides, optimize_direction ),
      _buffer( buffer )
  {
  }


  template <typename T> inline
  const_line_NDIterator<T>::const_line_NDIterator(
    const T* buffer, const std::vector<int> & dims,
    const std::vector<size_t> & strides, bool optimize_direction )
    : line_NDIterator_base( dims, strides, optimize_direction ),
      _buffer( buffer )
  {
  }


  template <typename T> inline
  const_line_NDIterator<T>::const_line_NDIterator(
    const T* buffer, const std::vector<int> & dims,
    const std::vector<long> & strides, bool optimize_direction )
    : line_NDIterator_base( dims, strides, optimize_direction ),
      _buffer( buffer )
  {
  }


  template <typename T> inline const T &
  const_line_NDIterator<T>::operator * () const
  {
    return _buffer[ _offset ];
  }


  template <typename T> inline
  void const_line_NDIterator<T>::inc_line_ptr( const T* & p ) const
  {
    if( _contiguous )
      ++p;
    else
      p += _line_stride;
  }


  template <typename T> inline
  long const_line_NDIterator<T>::line_length() const
  {
    if( is_contiguous() )
      return line_size();
    else
      return line_size() * _strides[_line_directon];
  }

}

#endif
