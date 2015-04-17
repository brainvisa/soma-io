#ifdef SOMA_IO_DICOM
#include <soma-io/Container/Matrix.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Container/Matrix.h>
#include <Utils/StdInt.h>
#endif

#include <cmath>


template < class T >
dcm::Matrix< T >::Matrix()
{

  int32_t i, j;

  for ( j = 0; j < 3; j++ )
  {
    for ( i = 0; i < 3; i++ )
    {

      m[ j ][ i ] = 0.0;

    }
  }

}

template < class T >
dcm::Matrix< T >::Matrix( const dcm::Matrix< T >& other )
{

  int32_t i, j;

  for ( j = 0; j < 3; j++ )
  {
    for ( i = 0; i < 3; i++ )
    {

      m[ j ][ i ] = other.m[ j ][ i ];

    }
  }

}


template < class T >
dcm::Matrix< T >::Matrix( T m00, T m01, T m02,
                          T m10, T m11, T m12,
                          T m20, T m21, T m22 )
{

  m[ 0 ][ 0 ] = m00;
  m[ 0 ][ 1 ] = m01;
  m[ 0 ][ 2 ] = m02;
  m[ 1 ][ 0 ] = m10;
  m[ 1 ][ 1 ] = m11;
  m[ 1 ][ 2 ] = m12;
  m[ 2 ][ 0 ] = m20;
  m[ 2 ][ 1 ] = m21;
  m[ 2 ][ 2 ] = m22;

}


template < class T >
dcm::Matrix< T >::Matrix( const dcm::Vector3d< T >& rowVec, 
                          const dcm::Vector3d< T >& colVec, 
                          const dcm::Vector3d< T >& normVec )
{

  m[ 0 ][ 0 ] = rowVec.x;
  m[ 0 ][ 1 ] = colVec.x;
  m[ 0 ][ 2 ] = normVec.x;
  m[ 1 ][ 0 ] = rowVec.y;
  m[ 1 ][ 1 ] = colVec.y;
  m[ 1 ][ 2 ] = normVec.y;
  m[ 2 ][ 0 ] = rowVec.z;
  m[ 2 ][ 1 ] = colVec.z;
  m[ 2 ][ 2 ] = normVec.z;

}


template < class T >
inline
dcm::Matrix< T > dcm::Matrix< T >::inverse()
{

  T det = m[ 0 ][ 0 ] * ( m[ 2 ][ 2 ] * m[ 1 ][ 1 ] - 
                          m[ 2 ][ 1 ] * m[ 1 ][ 2 ] )-
          m[ 1 ][ 0 ] * ( m[ 2 ][ 2 ] * m[ 0 ][ 1 ] -
	                  m[ 2 ][ 1 ] * m[ 0 ][ 2 ] )+
          m[ 2 ][ 0 ] * ( m[ 1 ][ 2 ] * m[ 0 ][ 1 ] -
	                  m[ 1 ][ 1 ] * m[ 0 ][ 2 ] );

  T i00 = ( m[ 2 ][ 2 ] * m[ 1 ][ 1 ] - m[ 2 ][ 1 ] * m[ 1 ][ 2 ] ) / det;
  T i01 = -( m[ 2 ][ 2 ] * m[ 0 ][ 1 ] - m[ 2 ][ 1 ] * m[ 0 ][ 2 ] ) / det;
  T i02 = ( m[ 1 ][ 2 ] * m[ 0 ][ 1 ] - m[ 1 ][ 1 ] * m[ 0 ][ 2 ] ) / det;
  T i10 = -( m[ 2 ][ 2 ] * m[ 1 ][ 0 ] - m[ 2 ][ 0 ] * m[ 1 ][ 2 ] ) / det;
  T i11 = ( m[ 2 ][ 2 ] * m[ 0 ][ 0 ] - m[ 2 ][ 0 ] * m[ 0 ][ 2 ] ) / det;
  T i12 = -( m[ 1 ][ 2 ] * m[ 0 ][ 0 ] - m[ 1 ][ 0 ] * m[ 0 ][ 2 ] ) / det;
  T i20 = ( m[ 2 ][ 1 ] * m[ 1 ][ 0 ] - m[ 2 ][ 0 ] * m[ 1 ][ 1 ] ) / det;
  T i21 = -( m[ 2 ][ 1 ] * m[ 0 ][ 0 ] - m[ 2 ][ 0 ] * m[ 0 ][ 1 ] ) / det;
  T i22 = ( m[ 1 ][ 1 ] * m[ 0 ][ 0 ] - m[ 1 ][ 0 ] * m[ 0 ][ 1 ] ) / det;

  return dcm::Matrix< T >( i00, i01, i02, i10, i11, i12, i20, i21, i22 );

}


template < class T >
inline
dcm::Matrix< T > operator * ( const dcm::Matrix< T >& mat1, 
                              const dcm::Matrix< T >& mat2 )
{

  dcm::Matrix< T > m;
  int32_t i, j, k;

  for ( j = 0; j < 3; j++ )
  {

    for ( i = 0; i < 3; i++ )
    {

      m.m[ j ][ i ] = 0.0;

      for ( k = 0; k < 3; k++ )
      {

        m.m[ j ][ i ] += mat1.m[ j ][ k ] * mat2.m[ k ][ i ];

      }

    }

  }

  return m;

}


template < class T >
inline
dcm::Vector3d< T > operator * ( const dcm::Matrix< T >& mat,
                                const dcm::Vector3d< T >& vec )
{

  dcm::Vector3d< T > v;
  
  v.x = mat.m[ 0 ][ 0 ] * vec.x + mat.m[ 0 ][ 1 ] * vec.y + 
        mat.m[ 0 ][ 2 ] * vec.z; 
  v.y = mat.m[ 1 ][ 0 ] * vec.x + mat.m[ 1 ][ 1 ] * vec.y + 
        mat.m[ 1 ][ 2 ] * vec.z;
  v.z = mat.m[ 2 ][ 0 ] * vec.x + mat.m[ 2 ][ 1 ] * vec.y + 
        mat.m[ 2 ][ 2 ] * vec.z;

  return v;

}


std::ostream& std::operator << ( std::ostream& os, 
                                 const dcm::Matrix< float >& m )
{

  os << "[ [" << m.m[ 0 ][ 0 ] << "," << m.m[ 0 ][ 1 ] << "," 
     << m.m[ 0 ][ 2 ] << "]" << std::endl
     << "  [" << m.m[ 1 ][ 0 ] << "," << m.m[ 1 ][ 1 ] << "," 
     << m.m[ 1 ][ 2 ] << "]" << std::endl
     << "  [" << m.m[ 2 ][ 0 ] << "," << m.m[ 2 ][ 1 ] << "," 
     << m.m[ 2 ][ 2 ] << "] ]";

  return os;

}


std::ostream& std::operator << ( std::ostream& os, 
                                 const dcm::Matrix< double >& m )
{

  os << "[ [" << m.m[ 0 ][ 0 ] << "," << m.m[ 0 ][ 1 ] << "," 
     << m.m[ 0 ][ 2 ] << "]" << std::endl
     << "  [" << m.m[ 1 ][ 0 ] << "," << m.m[ 1 ][ 1 ] << "," 
     << m.m[ 1 ][ 2 ] << "]" << std::endl
     << "  [" << m.m[ 2 ][ 0 ] << "," << m.m[ 2 ][ 1 ] << "," 
     << m.m[ 2 ][ 2 ] << "] ]";

  return os;

}
