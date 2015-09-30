#ifdef SOMA_IO_DICOM
#include <soma-io/Container/Vector3d.h>
#else
#include <Container/Vector3d.h>
#endif

#include <cmath>
#include <cassert>


template < class T >
dcm::Vector3d< T >::Vector3d()
                  : x( 0 ),
                    y( 0 ),
                    z( 0 )
{
}


template < class T >
dcm::Vector3d< T >::Vector3d( const dcm::Vector3d< T >& other )
                  : x( other.x ), 
                    y( other.y ), 
                    z( other.z )  
{
}


template < class T >
dcm::Vector3d< T >::Vector3d( T theX, T theY, T theZ )
                  : x( theX ),
                    y( theY ),
                    z( theZ )
{
}


template < class T >
inline
dcm::Vector3d< T >& dcm::Vector3d< T >::operator += ( 
                                               const dcm::Vector3d< T >& other )
{

  x += other.x;
  y += other.y;
  z += other.z;

  return *this;

}


template < class T >
inline
dcm::Vector3d< T >& dcm::Vector3d< T >::operator -= ( 
                                               const dcm::Vector3d< T >& other )
{

  x -= other.x;
  y -= other.y;
  z -= other.z;

  return *this;

}


template < class T >
inline
dcm::Vector3d< T >& dcm::Vector3d< T >::operator *= ( T value )
{

  x *= value;
  y *= value;
  z *= value;

  return *this;

}


template < class T >
inline
dcm::Vector3d< T >& dcm::Vector3d< T >::operator /= ( T value )
{

  assert( std::fabs( value ) > 1e-9 );
  x /= value;
  y /= value;
  z /= value;

  return *this;

}


template < class T >
inline
dcm::Vector3d< T > dcm::Vector3d< T >::cross( 
                                         const dcm::Vector3d< T >& other ) const
{

  return dcm::Vector3d< T >( y * other.z - z * other.y,
                             z * other.x - x * other.z,
                             x * other.y - y * other.x );

}


template < class T >
inline
T dcm::Vector3d< T >::dot( const dcm::Vector3d< T >& other )
{

  return x * other.x + y * other.y + z * other.z;

}


template < class T >
inline
T dcm::Vector3d< T >::getNorm()
{

  return std::sqrt( x * x + y * y + z * z );

}


template < class T >
inline
dcm::Vector3d< T >& dcm::Vector3d< T >::normalize()
{

  return this->operator /= ( getNorm() );

}


template < class T >
inline
dcm::Vector3d< T > dcm::Vector3d< T >::mainDirection() const
{

  dcm::Vector3d< T > mainVector( 0, 0, 0 );
  double absX = std::fabs( x );
  double absY = std::fabs( y );
  double absZ = std::fabs( z );

  if ( absX > absY )
  {

    if ( absX > absZ )
    {
  
      mainVector.x = x / absX;
  
    }
    else
    {
  
      mainVector.z = z / absZ;
  
    }

  }
  else
  {

    if ( absY > absZ )
    {
  
      mainVector.y = y / absY;
  
    }
    else
    {
   
      mainVector.z = z / absZ;
  
    }

  }

  return mainVector;

}


template < class T >
inline
dcm::Vector3d< T > dcm::Vector3d< T >::absoluteCoordinates() const
{

  return dcm::Vector3d< T >( std::fabs( x ), std::fabs( y ), std::fabs( z ) );

}


template < class T >
inline
dcm::Vector3d< T > operator + ( const dcm::Vector3d< T >& vec )
{

  return vec;

}


template < class T >
inline
dcm::Vector3d< T > operator - ( const dcm::Vector3d< T >& vec )
{

  return dcm::Vector3d< T >( -vec.x, -vec.y, -vec.z );

}


template < class T >
inline
dcm::Vector3d< T > operator + ( const dcm::Vector3d< T >& vec1,
                                const dcm::Vector3d< T >& vec2 )
{

  return dcm::Vector3d< T >( vec1.x + vec2.x, 
                             vec1.y + vec2.y,
                             vec1.z + vec2.z );

}


template < class T >
inline
dcm::Vector3d< T > operator - ( const dcm::Vector3d< T >& vec1,
                                const dcm::Vector3d< T >& vec2 )
{

  return dcm::Vector3d< T >( vec1.x - vec2.x,
                             vec1.y - vec2.y,
                             vec1.z - vec2.z );

}


template < class T >
inline
dcm::Vector3d< T > operator * ( const dcm::Vector3d< T >& vec, const T& val )
{

  return dcm::Vector3d< T >( val * vec.x, val * vec.y, val * vec.z );

}


template < class T >
inline
dcm::Vector3d< T > operator / ( const dcm::Vector3d< T >& vec, const T& val )
{

  return dcm::Vector3d< T >( vec.x / val, vec.y / val, vec.z / val );

}


std::ostream& std::operator << ( std::ostream& os, 
                                 const dcm::Vector3d< int32_t >& vec )
{

  os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";

  return os;

}


std::ostream& std::operator << ( std::ostream& os, 
                                 const dcm::Vector3d< float >& vec )
{

  os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";

  return os;

}


std::ostream& std::operator << ( std::ostream& os, 
                                 const dcm::Vector3d< double >& vec )
{

  os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";

  return os;

}


template class dcm::Vector3d< int32_t >;
template class dcm::Vector3d< float >;
template class dcm::Vector3d< double >;

template dcm::Vector3d< int32_t >
operator + ( const dcm::Vector3d< int32_t >& thing );
template dcm::Vector3d< float >
operator + ( const dcm::Vector3d< float >& thing );
template dcm::Vector3d< double >
operator + ( const dcm::Vector3d< double >& thing );

template dcm::Vector3d< int32_t >
operator - ( const dcm::Vector3d< int32_t >& thing );
template dcm::Vector3d< float >
operator - ( const dcm::Vector3d< float >& thing );
template dcm::Vector3d< double >
operator - ( const dcm::Vector3d< double >& thing );

template dcm::Vector3d< int32_t >
operator + ( const dcm::Vector3d< int32_t >& thing1,
             const dcm::Vector3d< int32_t >& thing2 );
template dcm::Vector3d< float >
operator + ( const dcm::Vector3d< float >& thing1,
             const dcm::Vector3d< float >& thing2 );
template dcm::Vector3d< double >
operator + ( const dcm::Vector3d< double >& thing1,
             const dcm::Vector3d< double >& thing2 );

template dcm::Vector3d< int32_t >
operator - ( const dcm::Vector3d< int32_t >& thing1,
             const dcm::Vector3d< int32_t >& thing2 );
template dcm::Vector3d< float >
operator - ( const dcm::Vector3d< float >& thing1,
             const dcm::Vector3d< float >& thing2 );
template dcm::Vector3d< double >
operator - ( const dcm::Vector3d< double >& thing1,
             const dcm::Vector3d< double >& thing2 );

template dcm::Vector3d< int32_t >
operator * ( const dcm::Vector3d< int32_t >& thing1,
             const int32_t& thing2 );
template dcm::Vector3d< float >
operator * ( const dcm::Vector3d< float >& thing1,
             const float& thing2 );
template dcm::Vector3d< double >
operator * ( const dcm::Vector3d< double >& thing1,
             const double& thing2 );

template dcm::Vector3d< int32_t >
operator / ( const dcm::Vector3d< int32_t >& thing1,
             const int32_t& thing2 );
template dcm::Vector3d< float >
operator / ( const dcm::Vector3d< float >& thing1,
             const float& thing2 );
template dcm::Vector3d< double >
operator / ( const dcm::Vector3d< double >& thing1,
             const double& thing2 );
