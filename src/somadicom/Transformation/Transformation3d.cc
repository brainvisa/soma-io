#ifdef SOMA_IO_DICOM
#include <soma-io/Transformation/Transformation3d.h>
#else
#include <Transformation/Transformation3d.h>
#endif

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

      _direct[ j ][ i ] = other._direct[ j ][ i ];
      _inverse[ j ][ i ] = other._inverse[ j ][ i ];

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

  _direct[ 0 ][ 0 ] = r00;
  _direct[ 0 ][ 1 ] = r01;
  _direct[ 0 ][ 2 ] = r02;
  _direct[ 0 ][ 3 ] = tx;
  _direct[ 1 ][ 0 ] = r10;
  _direct[ 1 ][ 1 ] = r11;
  _direct[ 1 ][ 2 ] = r12;
  _direct[ 1 ][ 3 ] = ty;
  _direct[ 2 ][ 0 ] = r20;
  _direct[ 2 ][ 1 ] = r21;
  _direct[ 2 ][ 2 ] = r22;
  _direct[ 2 ][ 3 ] = tz;
  _direct[ 3 ][ 0 ] = sx;
  _direct[ 3 ][ 1 ] = sy;
  _direct[ 3 ][ 2 ] = sz;
  _direct[ 3 ][ 3 ] = w;

  invert( _direct, _inverse );

}


void soma::Transformation3d::getDirect( 
                     const double fromX, const double fromY, const double fromZ,
                     double& toX, double& toY, double& toZ ) const
{

  double h = _direct[ 3 ][ 0 ] * fromX + 
             _direct[ 3 ][ 1 ] * fromY +
             _direct[ 3 ][ 2 ] * fromZ +
             _direct[ 3 ][ 3 ];

  toX = ( _direct[ 0 ][ 0 ] * fromX + 
          _direct[ 0 ][ 1 ] * fromY + 
          _direct[ 0 ][ 2 ] * fromZ +
          _direct[ 0 ][ 3 ] ) / h;
  toY = ( _direct[ 1 ][ 0 ] * fromX + 
          _direct[ 1 ][ 1 ] * fromY + 
          _direct[ 1 ][ 2 ] * fromZ +
          _direct[ 1 ][ 3 ] ) / h;
  toZ = ( _direct[ 2 ][ 0 ] * fromX + 
          _direct[ 2 ][ 1 ] * fromY + 
          _direct[ 2 ][ 2 ] * fromZ +
          _direct[ 2 ][ 3 ] ) / h;

}


void soma::Transformation3d::getInverse( 
                           const double toX, const double toY, const double toZ,
                           double& fromX, double& fromY, double& fromZ ) const
{

  double h = _inverse[ 3 ][ 0 ] * toX + 
             _inverse[ 3 ][ 1 ] * toY +
             _inverse[ 3 ][ 2 ] * toZ +
             _inverse[ 3 ][ 3 ];

  fromX = ( _inverse[ 0 ][ 0 ] * toX + 
            _inverse[ 0 ][ 1 ] * toY + 
            _inverse[ 0 ][ 2 ] * toZ +
            _inverse[ 0 ][ 3 ] ) / h;
  fromY = ( _inverse[ 1 ][ 0 ] * toX + 
            _inverse[ 1 ][ 1 ] * toY + 
            _inverse[ 1 ][ 2 ] * toZ +
            _inverse[ 1 ][ 3 ] ) / h;
  fromZ = ( _inverse[ 2 ][ 0 ] * toX + 
            _inverse[ 2 ][ 1 ] * toY + 
            _inverse[ 2 ][ 2 ] * toZ +
            _inverse[ 2 ][ 3 ] ) / h;

}


double soma::Transformation3d::getDirectCoefficient( int32_t j, 
                                                     int32_t i ) const
{

  assert( ( i >= 0 ) && ( i < 4 ) && ( j >= 0 ) && ( j < 4 ) );

  return _direct[ j ][ i ];

}


double soma::Transformation3d::getInverseCoefficient( int32_t j, 
                                                      int32_t i ) const
{

  assert( ( i >= 0 ) && ( i < 4 ) && ( j >= 0 ) && ( j < 4 ) );

  return _inverse[ j ][ i ];

}


std::vector< double > soma::Transformation3d::getDirectCoefficients() const
{

  std::vector< double > coef( 16 );

  coef[ 0 ] = _direct[ 0 ][ 0 ];
  coef[ 1 ] = _direct[ 0 ][ 1 ];
  coef[ 2 ] = _direct[ 0 ][ 2 ];
  coef[ 3 ] = _direct[ 0 ][ 3 ];

  coef[ 4 ] = _direct[ 1 ][ 0 ];
  coef[ 5 ] = _direct[ 1 ][ 1 ];
  coef[ 6 ] = _direct[ 1 ][ 2 ];
  coef[ 7 ] = _direct[ 1 ][ 3 ];

  coef[ 8 ] = _direct[ 2 ][ 0 ];
  coef[ 9 ] = _direct[ 2 ][ 1 ];
  coef[ 10 ] = _direct[ 2 ][ 2 ];
  coef[ 11 ] = _direct[ 2 ][ 3 ];

  coef[ 12 ] = _direct[ 3 ][ 0 ];
  coef[ 13 ] = _direct[ 3 ][ 1 ];
  coef[ 14 ] = _direct[ 3 ][ 2 ];
  coef[ 15 ] = _direct[ 3 ][ 3 ];

  return coef;

}


soma::Transformation3d soma::Transformation3d::inverse() const
{

  soma::Transformation3d t;
  int32_t i, j;
  
  for ( j = 0; j < 4; j++ )
  {
  
    for ( i = 0; i < 4; i++ )
    {
    
      t._direct[ j ][ i ] = _inverse[ j ][ i ];
      t._inverse[ j ][ i ] = _direct[ j ][ i ];
    
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
