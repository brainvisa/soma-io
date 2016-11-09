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
#include <sys/types.h>


namespace carto
{

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
    off_t offset() const;

    NDIterator_base & operator ++();

    bool ended() const;
    void reset();

  protected:
    template <typename T>
    static std::vector<size_t> to_int_v( const std::vector<T> & v );

    std::vector<int> _dims;
    std::vector<size_t> _strides;
    std::vector<int> _position;
    off_t _offset;
    bool _ended;
  };


  class line_NDIterator_base : public NDIterator_base
  {
  public:
    line_NDIterator_base( const std::vector<int> & dims );
    line_NDIterator_base( const std::vector<int> & dims,
                          const std::vector<int> & strides );
    line_NDIterator_base( const std::vector<int> & dims,
                          const std::vector<size_t> & strides );
    line_NDIterator_base( const std::vector<int> & dims,
                          const std::vector<long> & strides );
    // virtual ~line_NDIterator_base() {}

    NDIterator_base & operator ++();
  };


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


  template <typename T> class line_NDIterator : public line_NDIterator_base
  {
  public:
    line_NDIterator( T* buffer, const std::vector<int> & dims );
    line_NDIterator( T* buffer, const std::vector<int> & dims,
                     const std::vector<int> & strides );
    line_NDIterator( T* buffer, const std::vector<int> & dims,
                     const std::vector<size_t> & strides );
    line_NDIterator( T* buffer, const std::vector<int> & dims,
                     const std::vector<long> & strides );
    // virtual ~line_NDIterator() {}

    T & operator * () const;

  protected:
    T* _buffer;
  };


  template <typename T> class const_line_NDIterator
    : public line_NDIterator_base
  {
  public:
    const_line_NDIterator( const T* buffer, const std::vector<int> & dims );
    const_line_NDIterator( const T* buffer, const std::vector<int> & dims,
                           const std::vector<int> & strides );
    const_line_NDIterator( const T* buffer, const std::vector<int> & dims,
                           const std::vector<size_t> & strides );
    const_line_NDIterator( const T* buffer, const std::vector<int> & dims,
                           const std::vector<long> & strides );
    // virtual ~const_line_NDIterator() {}

    const T & operator * () const;

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


  inline off_t NDIterator_base::offset() const
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
    : NDIterator_base( dims )
  {
    if( dims.size() < 2 )
      _ended = true;
  }


  inline line_NDIterator_base::line_NDIterator_base(
    const std::vector<int> & dims, const std::vector<int> & strides )
    : NDIterator_base( dims, strides )
  {
    if( dims.size() < 2 )
      _ended = true;
  }


  inline line_NDIterator_base::line_NDIterator_base(
    const std::vector<int> & dims, const std::vector<size_t> & strides )
    : NDIterator_base( dims, strides )
  {
    if( dims.size() < 2 )
      _ended = true;
  }


  inline line_NDIterator_base::line_NDIterator_base(
    const std::vector<int> & dims, const std::vector<long> & strides )
    : NDIterator_base( dims, strides )
  {
    if( dims.size() < 2 )
      _ended = true;
  }


  inline NDIterator_base & line_NDIterator_base::operator ++()
  {
    size_t dim, ndim = _dims.size();
    bool nextrow = true, stride = !_strides.empty();
    for( dim=1; nextrow && dim<ndim; ++dim )
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
  line_NDIterator<T>::line_NDIterator( T* buffer, const std::vector<int> & dims,
                                       const std::vector<int> & strides )
    : line_NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> inline
  line_NDIterator<T>::line_NDIterator( T* buffer, const std::vector<int> & dims,
                                       const std::vector<size_t> & strides )
    : line_NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> inline
  line_NDIterator<T>::line_NDIterator( T* buffer, const std::vector<int> & dims,
                                       const std::vector<long> & strides )
    : line_NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> inline T & line_NDIterator<T>::operator * () const
  {
    return _buffer[ _offset ];
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
    const std::vector<int> & strides )
    : line_NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> inline
  const_line_NDIterator<T>::const_line_NDIterator(
    const T* buffer, const std::vector<int> & dims,
    const std::vector<size_t> & strides )
    : line_NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> inline
  const_line_NDIterator<T>::const_line_NDIterator(
    const T* buffer, const std::vector<int> & dims,
    const std::vector<long> & strides )
    : line_NDIterator_base( dims, strides ), _buffer( buffer )
  {
  }


  template <typename T> inline const T &
  const_line_NDIterator<T>::operator * () const
  {
    return _buffer[ _offset ];
  }

}

#endif
