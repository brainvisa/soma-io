#ifdef SOMA_IO_DICOM
#include <soma-io/Container/Matrix.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Container/Matrix.h>
#include <Utils/StdInt.h>
#endif

#include <cmath>


soma::Matrix::Matrix()
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

soma::Matrix::Matrix( const soma::Matrix& other )
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


soma::Matrix::Matrix( double m00, double m01, double m02,
                      double m10, double m11, double m12,
                       double m20, double m21, double m22 )
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


soma::Matrix::Matrix( const soma::Vector& rowVec, 
                      const soma::Vector& colVec, 
                      const soma::Vector& normVec )
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


soma::Matrix soma::Matrix::inverse()
{

  double det = m[ 0 ][ 0 ] * ( m[ 2 ][ 2 ] * m[ 1 ][ 1 ] - 
                               m[ 2 ][ 1 ] * m[ 1 ][ 2 ] )-
               m[ 1 ][ 0 ] * ( m[ 2 ][ 2 ] * m[ 0 ][ 1 ] -
	                       m[ 2 ][ 1 ] * m[ 0 ][ 2 ] )+
               m[ 2 ][ 0 ] * ( m[ 1 ][ 2 ] * m[ 0 ][ 1 ] -
	                       m[ 1 ][ 1 ] * m[ 0 ][ 2 ] );

  double i00 = ( m[ 2 ][ 2 ] * m[ 1 ][ 1 ] - m[ 2 ][ 1 ] * m[ 1 ][ 2 ] ) / det;
  double i01 = -( m[ 2 ][ 2 ] * m[ 0 ][ 1 ] - m[ 2 ][ 1 ] * m[ 0 ][ 2 ] ) / det;
  double i02 = ( m[ 1 ][ 2 ] * m[ 0 ][ 1 ] - m[ 1 ][ 1 ] * m[ 0 ][ 2 ] ) / det;
  double i10 = -( m[ 2 ][ 2 ] * m[ 1 ][ 0 ] - m[ 2 ][ 0 ] * m[ 1 ][ 2 ] ) / det;
  double i11 = ( m[ 2 ][ 2 ] * m[ 0 ][ 0 ] - m[ 2 ][ 0 ] * m[ 0 ][ 2 ] ) / det;
  double i12 = -( m[ 1 ][ 2 ] * m[ 0 ][ 0 ] - m[ 1 ][ 0 ] * m[ 0 ][ 2 ] ) / det;
  double i20 = ( m[ 2 ][ 1 ] * m[ 1 ][ 0 ] - m[ 2 ][ 0 ] * m[ 1 ][ 1 ] ) / det;
  double i21 = -( m[ 2 ][ 1 ] * m[ 0 ][ 0 ] - m[ 2 ][ 0 ] * m[ 0 ][ 1 ] ) / det;
  double i22 = ( m[ 1 ][ 1 ] * m[ 0 ][ 0 ] - m[ 1 ][ 0 ] * m[ 0 ][ 1 ] ) / det;

  return soma::Matrix( i00, i01, i02, i10, i11, i12, i20, i21, i22 );

}


soma::Matrix operator * ( const soma::Matrix& mat1, const soma::Matrix& mat2 )
{

  soma::Matrix m;
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


soma::Vector operator * ( const soma::Matrix& mat, const soma::Vector& vec )
{

  soma::Vector v;
  
  v.x = mat.m[ 0 ][ 0 ] * vec.x + mat.m[ 0 ][ 1 ] * vec.y + 
        mat.m[ 0 ][ 2 ] * vec.z; 
  v.y = mat.m[ 1 ][ 0 ] * vec.x + mat.m[ 1 ][ 1 ] * vec.y + 
        mat.m[ 1 ][ 2 ] * vec.z;
  v.z = mat.m[ 2 ][ 0 ] * vec.x + mat.m[ 2 ][ 1 ] * vec.y + 
        mat.m[ 2 ][ 2 ] * vec.z;

  return v;

}


std::ostream& std::operator << ( std::ostream& os, const soma::Matrix& m )
{

  os << "[ [" << m.m[ 0 ][ 0 ] << "," << m.m[ 0 ][ 1 ] << "," 
     << m.m[ 0 ][ 2 ] << "]" << std::endl
     << "  [" << m.m[ 1 ][ 0 ] << "," << m.m[ 1 ][ 1 ] << "," 
     << m.m[ 1 ][ 2 ] << "]" << std::endl
     << "  [" << m.m[ 2 ][ 0 ] << "," << m.m[ 2 ][ 1 ] << "," 
     << m.m[ 2 ][ 2 ] << "] ]";

  return os;

}
