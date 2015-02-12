#ifdef SOMA_IO_DICOM
#include <soma-io/Container/BoundingBox.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Container/BoundingBox.h>
#include <Utils/StdInt.h>
#endif


template < class T >
soma::BoundingBox< T >::BoundingBox( const T& lowerX, const T& upperX,
                                     const T& lowerY, const T& upperY,
                                     const T& lowerZ, const T& upperZ,
                                     const T& lowerT, const T& upperT )
                      : _lowerX( lowerX ),
                        _upperX( upperX ),
                        _lowerY( lowerY ),
                        _upperY( upperY ),
                        _lowerZ( lowerZ ),
                        _upperZ( upperZ ),
                        _lowerT( lowerT ),
                        _upperT( upperT )
{
}


template < class T >
soma::BoundingBox< T >::BoundingBox( const soma::BoundingBox< T >& other )
                      : _lowerX( other._lowerX ),
                        _upperX( other._upperX ),
                        _lowerY( other._lowerY ),
                        _upperY( other._upperY ),
                        _lowerZ( other._lowerZ ),
                        _upperZ( other._upperZ ),
                        _lowerT( other._lowerT ),
                        _upperT( other._upperT )
{
}


template < class T >
soma::BoundingBox< T >::~BoundingBox()
{
}


template < class T >
void soma::BoundingBox< T >::setLowerX( const T& lowerX )
{

  _lowerX = lowerX;

}


template < class T >
const T& soma::BoundingBox< T >::getLowerX() const
{

  return _lowerX;

}


template < class T >
void soma::BoundingBox< T >::setUpperX( const T& upperX )
{

  _upperX = upperX;

}


template < class T >
const T& soma::BoundingBox< T >::getUpperX() const
{

  return _upperX;

}


template < class T >
void soma::BoundingBox< T >::setLowerY( const T& lowerY )
{

  _lowerY = lowerY;

}


template < class T >
const T& soma::BoundingBox< T >::getLowerY() const
{

  return _lowerY;

}


template < class T >
void soma::BoundingBox< T >::setUpperY( const T& upperY )
{

  _upperY = upperY;

}


template < class T >
const T& soma::BoundingBox< T >::getUpperY() const
{

  return _upperY;

}


template < class T >
void soma::BoundingBox< T >::setLowerZ( const T& lowerZ )
{

  _lowerZ = lowerZ;

}


template < class T >
const T& soma::BoundingBox< T >::getLowerZ() const
{

  return _lowerZ;

}


template < class T >
void soma::BoundingBox< T >::setUpperZ( const T& upperZ )
{

  _upperZ = upperZ;

}


template < class T >
const T& soma::BoundingBox< T >::getUpperZ() const
{

  return _upperZ;

}


template < class T >
void soma::BoundingBox< T >::setLowerT( const T& lowerT )
{

  _lowerT = lowerT;

}


template < class T >
const T& soma::BoundingBox< T >::getLowerT() const
{

  return _lowerT;

}


template < class T >
void soma::BoundingBox< T >::setUpperT( const T& upperT )
{

  _upperT = upperT;

}


template < class T >
const T& soma::BoundingBox< T >::getUpperT() const
{

  return _upperT;

}


template < class T >
bool soma::BoundingBox< T >::contains( const T& x,
                                       const T& y,
                                       const T& z,
                                       const T& t ) const
{

  return ( x >= _lowerX ) && ( x <= _upperX ) &&
         ( y >= _lowerY ) && ( y <= _upperY ) &&
         ( z >= _lowerZ ) && ( z <= _upperZ ) &&
         ( t >= _lowerT ) && ( t <= _upperT );

}


template < class T >
bool soma::BoundingBox< T >::isOnBoundary( const T& x,
                                           const T& y,
                                           const T& z,
                                           const T& t ) const
{

  bool result = false;

  // the bounding box is 3D
  if ( _upperT == 0 )
  {

    // the bounding box is 2D
    if ( _upperZ == 0 )
    {

      result = ( ( x == _lowerX ) || ( x == _upperX ) ||
                 ( y == _lowerY ) || ( y == _upperY ) ) &&
               contains( x, y, z, t );

    }
    // the bounding box is really 3D
    else
    {

      result = ( ( x == _lowerX ) || ( x == _upperX ) ||
                 ( y == _lowerY ) || ( y == _upperY ) ||
                 ( z == _lowerZ ) || ( z == _upperZ ) ) &&
               contains( x, y, z, t );

    }

  }
  // else it is a 4D container
  else
  {

    result = ( ( x == _lowerX ) || ( x == _upperX ) ||
               ( y == _lowerY ) || ( y == _upperY ) ||
               ( z == _lowerZ ) || ( z == _upperZ ) ||
               ( t == _lowerT ) || ( t == _upperT ) ) &&
             contains( x, y, z, t );

  }

  return result;

}


template class soma::BoundingBox< int8_t >;
template class soma::BoundingBox< int16_t >;
template class soma::BoundingBox< int32_t >;
template class soma::BoundingBox< int64_t >;

