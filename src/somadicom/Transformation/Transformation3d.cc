#include <soma-io/Transformation/Transformation3d.h>
#include <soma-io/Utils/StdInt.h>

#include <cassert>


soma::Transformation3d::Transformation3d()
{

  setItems( 1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0 );

}


soma::Transformation3d::Transformation3d( const Transformation3d& other )
{

  int i, j;

  for ( j = 0; j < 4; j++ )
  {

    for ( i = 0; i < 4; i++ )
    {

      m_direct[ j ][ i ] = other.m_direct[ j ][ i ];
      m_inverse[ j ][ i ] = other.m_inverse[ j ][ i ];

    }

  }

}


soma::Transformation3d::~Transformation3d()
{
}


void soma::Transformation3d::setItems( 
                                  double r00, double r01, double r02, double tx,
                                  double r10, double r11, double r12, double ty,
                                  double r20, double r21, double r22, double tz,
                                  double sx, double sy, double sz, double w )
{

  m_direct[ 0 ][ 0 ] = r00;
  m_direct[ 0 ][ 1 ] = r01;
  m_direct[ 0 ][ 2 ] = r02;
  m_direct[ 0 ][ 3 ] = tx;
  m_direct[ 1 ][ 0 ] = r10;
  m_direct[ 1 ][ 1 ] = r11;
  m_direct[ 1 ][ 2 ] = r12;
  m_direct[ 1 ][ 3 ] = ty;
  m_direct[ 2 ][ 0 ] = r20;
  m_direct[ 2 ][ 1 ] = r21;
  m_direct[ 2 ][ 2 ] = r22;
  m_direct[ 2 ][ 3 ] = tz;
  m_direct[ 3 ][ 0 ] = sx;
  m_direct[ 3 ][ 1 ] = sy;
  m_direct[ 3 ][ 2 ] = sz;
  m_direct[ 3 ][ 3 ] = w;

  invert( m_direct, m_inverse );

}


void soma::Transformation3d::getDirect( 
                     const double fromX, const double fromY, const double fromZ,
                     double& toX, double& toY, double& toZ )
{

  double h = m_direct[ 3 ][ 0 ] * fromX + 
             m_direct[ 3 ][ 1 ] * fromY +
             m_direct[ 3 ][ 2 ] * fromZ +
             m_direct[ 3 ][ 3 ];

  toX = ( m_direct[ 0 ][ 0 ] * fromX + 
          m_direct[ 0 ][ 1 ] * fromY + 
          m_direct[ 0 ][ 2 ] * fromZ +
          m_direct[ 0 ][ 3 ] ) / h;
  toY = ( m_direct[ 1 ][ 0 ] * fromX + 
          m_direct[ 1 ][ 1 ] * fromY + 
          m_direct[ 1 ][ 2 ] * fromZ +
          m_direct[ 1 ][ 3 ] ) / h;
  toZ = ( m_direct[ 2 ][ 0 ] * fromX + 
          m_direct[ 2 ][ 1 ] * fromY + 
          m_direct[ 2 ][ 2 ] * fromZ +
          m_direct[ 2 ][ 3 ] ) / h;

}


void soma::Transformation3d::getInverse( 
                           const double toX, const double toY, const double toZ,
                           double& fromX, double& fromY, double& fromZ )
{

  double h = m_inverse[ 3 ][ 0 ] * toX + 
             m_inverse[ 3 ][ 1 ] * toY +
             m_inverse[ 3 ][ 2 ] * toZ +
             m_inverse[ 3 ][ 3 ];

  fromX = ( m_inverse[ 0 ][ 0 ] * toX + 
            m_inverse[ 0 ][ 1 ] * toY + 
            m_inverse[ 0 ][ 2 ] * toZ +
            m_inverse[ 0 ][ 3 ] ) / h;
  fromY = ( m_inverse[ 1 ][ 0 ] * toX + 
            m_inverse[ 1 ][ 1 ] * toY + 
            m_inverse[ 1 ][ 2 ] * toZ +
            m_inverse[ 1 ][ 3 ] ) / h;
  fromZ = ( m_inverse[ 2 ][ 0 ] * toX + 
            m_inverse[ 2 ][ 1 ] * toY + 
            m_inverse[ 2 ][ 2 ] * toZ +
            m_inverse[ 2 ][ 3 ] ) / h;

}


double soma::Transformation3d::getDirectCoefficient( int32_t j, 
                                                     int32_t i ) const
{

  assert( ( i >= 0 ) && ( i < 4 ) && ( j >= 0 ) && ( j < 4 ) );

  return m_direct[ j ][ i ];

}


double soma::Transformation3d::getInverseCoefficient( int32_t j, 
                                                      int32_t i ) const
{

  assert( ( i >= 0 ) && ( i < 4 ) && ( j >= 0 ) && ( j < 4 ) );

  return m_inverse[ j ][ i ];

}


soma::Transformation3d soma::Transformation3d::inverse() const
{

  soma::Transformation3d t;
  int32_t i, j;
  
  for ( j = 0; j < 4; j++ )
  {
  
    for ( i = 0; i < 4; i++ )
    {
    
      t.m_direct[ j ][ i ] = m_inverse[ j ][ i ];
      t.m_inverse[ j ][ i ] = m_direct[ j ][ i ];
    
    }
  
  }
  
  return t;

}


void soma::Transformation3d::invert( double d[ 4 ][ 4 ], double i[ 4 ][ 4 ] )
{

  double determinant = d[ 0 ][ 0 ] *
                     ( d[ 1 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 1 ][ 2 ] ) -
                     d[ 1 ][ 0 ] *
                     ( d[ 0 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 0 ][ 2 ] ) +
                     d[ 2 ][ 0 ] *
                     ( d[ 0 ][ 1 ] * d[ 1 ][ 2 ] - d[ 1 ][ 1 ] * d[ 0 ][ 2 ] );

  i[ 0 ][ 0 ] = ( d[ 1 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 1 ][ 2 ]  ) /  
                determinant;                                                   
  i[ 1 ][ 0 ] = -( d[ 1 ][ 0 ] * d[ 2 ][ 2 ] - d[ 2 ][ 0 ] * d[ 1 ][ 2 ]  ) /  
                determinant;                                                   
  i[ 2 ][ 0 ] = ( d[ 1 ][ 0 ] * d[ 2 ][ 1 ] - d[ 2 ][ 0 ] * d[ 1 ][ 1 ]  ) /  
                determinant;                                                   
  i[ 3 ][ 0 ] = 0.0;                                                           

  i[ 0 ][ 1 ] = -( d[ 0 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 0 ][ 2 ]  ) /  
                determinant;                                                   
  i[ 1 ][ 1 ] = ( d[ 0 ][ 0 ] * d[ 2 ][ 2 ] - d[ 2 ][ 0 ] * d[ 0 ][ 2 ]  ) /  
                determinant;                                                   
  i[ 2 ][ 1 ] = -( d[ 0 ][ 0 ] * d[ 2 ][ 1 ] - d[ 2 ][ 0 ] * d[ 0 ][ 1 ]  ) /  
                determinant;                                                   
  i[ 3 ][ 1 ] = 0.0;                                                           

  i[ 0 ][ 2 ] = ( d[ 0 ][ 1 ] * d[ 1 ][ 2 ] - d[ 1 ][ 1 ] * d[ 0 ][ 2 ]  ) /  
                determinant;                                                   
  i[ 1 ][ 2 ] = -( d[ 0 ][ 0 ] * d[ 1 ][ 2 ] - d[ 1 ][ 0 ] * d[ 0 ][ 2 ]  ) /  
                determinant;                                                   
  i[ 2 ][ 2 ] = ( d[ 0 ][ 0 ] * d[ 1 ][ 1 ] - d[ 1 ][ 0 ] * d[ 0 ][ 1 ]  ) /  
                determinant;                                                   
  i[ 3 ][ 2 ] = 0.0;                                                           

  i[ 0 ][ 3 ] = -( d[ 0 ][ 1 ] *                                              
                   ( d[ 1 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 1 ][ 3 ] ) -
                   d[ 1 ][ 1 ] *                                              
                   ( d[ 0 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 0 ][ 3 ] ) + 
                   d[ 2 ][ 1 ] *                                              
                   ( d[ 0 ][ 2 ] * d[ 1 ][ 3 ] - d[ 1 ][ 2 ] * d[ 0 ][ 3 ] )   
                 ) / determinant;
  i[ 1 ][ 3 ] = ( d[ 0 ][ 0 ] *                                              
                  ( d[ 1 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 1 ][ 3 ] ) -
                  d[ 1 ][ 0 ] *                                              
                  ( d[ 0 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 0 ][ 3 ] ) +
                  d[ 2 ][ 0 ] *                                              
                  ( d[ 0 ][ 2 ] * d[ 1 ][ 3 ] - d[ 1 ][ 2 ] * d[ 0 ][ 3 ] )   
                ) / determinant;
  i[ 2 ][ 3 ] = -( d[ 0 ][ 0 ] *                                              
                   ( d[ 1 ][ 1 ] * d[ 2 ][ 3 ] - d[ 2 ][ 1 ] * d[ 1 ][ 3 ] ) -
                   d[ 1 ][ 0 ] *                                              
                   ( d[ 0 ][ 1 ] * d[ 2 ][ 3 ] - d[ 2 ][ 1 ] * d[ 0 ][ 3 ] ) +
                   d[ 2 ][ 0 ] *                                              
                   ( d[ 0 ][ 1 ] * d[ 1 ][ 3 ] - d[ 1 ][ 1 ] * d[ 0 ][ 3 ] )   
                 ) / determinant;
  i[ 3 ][ 3 ] = ( d[ 0 ][ 0 ] *                                              
                  ( d[ 1 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 1 ][ 2 ] ) -
                  d[ 1 ][ 0 ] *                                              
                  ( d[ 0 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 0 ][ 2 ] ) +
                  d[ 2 ][ 0 ] *                                              
                  ( d[ 0 ][ 1 ] * d[ 1 ][ 2 ] - d[ 1 ][ 1 ] * d[ 0 ][ 2 ] )   
                ) / determinant;

  int32_t ii, jj;
  for ( jj = 0; jj < 4; jj++ )
  {

    for ( ii = 0; ii < 4; ii++ )
    {

      i[ jj ][ ii ] /= i[ 3 ][ 3 ];

    }

  }

}


soma::Transformation3d operator * ( const soma::Transformation3d& t1,
                                    const soma::Transformation3d& t2 )
{

  soma::Transformation3d t;
  double m[ 4 ][ 4 ];
  int32_t i, j, k;

  for ( j = 0; j < 4; j++ )
  {

    for ( i = 0; i < 4; i++ )
    {

      m[ j ][ i ] = 0.0;

      for ( k = 0; k < 4; k++ )
      {

        m[ j ][ i ] += t1.getDirectCoefficient( j, k ) * 
	               t2.getDirectCoefficient( k, i );

      }

    }

  }
  
  t.setItems( m[ 0 ][ 0 ], m[ 0 ][ 1 ], m[ 0 ][ 2 ], m[ 0 ][ 3 ],
              m[ 1 ][ 0 ], m[ 1 ][ 1 ], m[ 1 ][ 2 ], m[ 1 ][ 3 ],
              m[ 2 ][ 0 ], m[ 2 ][ 1 ], m[ 2 ][ 2 ], m[ 2 ][ 3 ],
              m[ 3 ][ 0 ], m[ 3 ][ 1 ], m[ 3 ][ 2 ], m[ 3 ][ 3 ] );
  
  return t;

}


std::ostream& std::operator << ( std::ostream& os, 
                                 const soma::Transformation3d& t )
{

  int32_t i = 0;
  int32_t j = 0;

  for ( i = 0; i < 4; i++ )
  {

    for ( j = 0; j < 4; j++ )
    {

      os << t.getDirectCoefficient( i, j ) << " ";

    }

    if ( j != 3 )
    {

      os << std::endl;

    }

  }

  return os;  

}
