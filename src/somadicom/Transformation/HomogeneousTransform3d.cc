#ifdef SOMA_IO_DICOM
#include <soma-io/Transformation/HomogeneousTransform3d.h>
#else
#include <Transformation/HomogeneousTransform3d.h>
#endif

#include <cassert>


template < class T >
dcm::HomogeneousTransform3d< T >::HomogeneousTransform3d()
{

  setDirectItems( 1.0, 0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                  0.0, 0.0, 0.0, 1.0 );

}


template < class T >
dcm::HomogeneousTransform3d< T >::HomogeneousTransform3d( 
                                 const dcm::HomogeneousTransform3d< T >& other )
{

  int i, j;

  for ( j = 0; j < 4; j++ )
  {

    for ( i = 0; i < 4; i++ )
    {

      _directItems[ j ][ i ] = other._directItems[ j ][ i ];
      _inverseItems[ j ][ i ] = other._inverseItems[ j ][ i ];

    }

  }

}


template < class T >
dcm::HomogeneousTransform3d< T >::~HomogeneousTransform3d()
{
}


template < class T >
inline
void dcm::HomogeneousTransform3d< T >::setDirectItems( 
                                                      T r00, T r01, T r02, T tx,
                                                      T r10, T r11, T r12, T ty,
                                                      T r20, T r21, T r22, T tz,
                                                      T sx,  T sy,  T sz,  T w )
{

  _directItems[ 0 ][ 0 ] = r00;
  _directItems[ 0 ][ 1 ] = r01;
  _directItems[ 0 ][ 2 ] = r02;
  _directItems[ 0 ][ 3 ] = tx;
  _directItems[ 1 ][ 0 ] = r10;
  _directItems[ 1 ][ 1 ] = r11;
  _directItems[ 1 ][ 2 ] = r12;
  _directItems[ 1 ][ 3 ] = ty;
  _directItems[ 2 ][ 0 ] = r20;
  _directItems[ 2 ][ 1 ] = r21;
  _directItems[ 2 ][ 2 ] = r22;
  _directItems[ 2 ][ 3 ] = tz;
  _directItems[ 3 ][ 0 ] = sx;
  _directItems[ 3 ][ 1 ] = sy;
  _directItems[ 3 ][ 2 ] = sz;
  _directItems[ 3 ][ 3 ] = w;

  invert( _directItems, _inverseItems );

}


template < class T >
inline
void dcm::HomogeneousTransform3d< T >::getDirect( 
                                                 const dcm::Vector3d< T >& from, 
                                                 dcm::Vector3d< T >& to ) const
{

  double h = _directItems[ 3 ][ 0 ] * from.x + 
             _directItems[ 3 ][ 1 ] * from.y +
             _directItems[ 3 ][ 2 ] * from.z +
             _directItems[ 3 ][ 3 ];

  to.x = ( _directItems[ 0 ][ 0 ] * from.x + 
           _directItems[ 0 ][ 1 ] * from.y + 
           _directItems[ 0 ][ 2 ] * from.z +
           _directItems[ 0 ][ 3 ] ) / h;
  to.y = ( _directItems[ 1 ][ 0 ] * from.x + 
           _directItems[ 1 ][ 1 ] * from.y + 
           _directItems[ 1 ][ 2 ] * from.z +
           _directItems[ 1 ][ 3 ] ) / h;
  to.z = ( _directItems[ 2 ][ 0 ] * from.x + 
           _directItems[ 2 ][ 1 ] * from.y + 
           _directItems[ 2 ][ 2 ] * from.z +
           _directItems[ 2 ][ 3 ] ) / h;

}


template < class T >
void dcm::HomogeneousTransform3d< T >::getInverse(
                                                const dcm::Vector3d< T >& to, 
                                                dcm::Vector3d< T >& from ) const
{

  double h = _inverseItems[ 3 ][ 0 ] * to.x + 
             _inverseItems[ 3 ][ 1 ] * to.y +
             _inverseItems[ 3 ][ 2 ] * to.z +
             _inverseItems[ 3 ][ 3 ];

  from.x = ( _inverseItems[ 0 ][ 0 ] * to.x + 
             _inverseItems[ 0 ][ 1 ] * to.y + 
             _inverseItems[ 0 ][ 2 ] * to.z +
             _inverseItems[ 0 ][ 3 ] ) / h;
  from.y = ( _inverseItems[ 1 ][ 0 ] * to.x + 
             _inverseItems[ 1 ][ 1 ] * to.y + 
             _inverseItems[ 1 ][ 2 ] * to.z +
             _inverseItems[ 1 ][ 3 ] ) / h;
  from.z = ( _inverseItems[ 2 ][ 0 ] * to.x + 
             _inverseItems[ 2 ][ 1 ] * to.y + 
             _inverseItems[ 2 ][ 2 ] * to.z +
             _inverseItems[ 2 ][ 3 ] ) / h;

}


template < class T >
void dcm::HomogeneousTransform3d< T >::getFullIntegerDirect( 
                                           const dcm::Vector3d< int32_t >& from, 
                                           dcm::Vector3d< int32_t >& to ) const
{

  dcm::Vector3d< T > in( from.x, from.y, from.z );
  dcm::Vector3d< T > out;

  this->getDirect( in, out );

  to.x = int32_t( out.x + 0.5 );
  to.y = int32_t( out.y + 0.5 );
  to.z = int32_t( out.z + 0.5 );

}


template < class T >
inline
void dcm::HomogeneousTransform3d< T >::getFullIntegerInverse(
                                          const dcm::Vector3d< int32_t >& to, 
                                          dcm::Vector3d< int32_t >& from ) const
{

  dcm::Vector3d< T > in( to.x, to.y, to.z );
  dcm::Vector3d< T > out;

  getInverse( in, out );

  from.x = int32_t( out.x + 0.5 );
  from.y = int32_t( out.y + 0.5 );
  from.z = int32_t( out.z + 0.5 );

}


template < class T >
inline
T dcm::HomogeneousTransform3d< T >::getDirectCoefficient( int32_t j, 
                                                          int32_t i ) const
{

  assert( ( i >= 0 ) && ( i < 4 ) && ( j >= 0 ) && ( j < 4 ) );

  return _directItems[ j ][ i ];

}


template < class T >
inline
T dcm::HomogeneousTransform3d< T >::getInverseCoefficient( int32_t j, 
                                                           int32_t i ) const
{

  assert( ( i >= 0 ) && ( i < 4 ) && ( j >= 0 ) && ( j < 4 ) );

  return _inverseItems[ j ][ i ];

}


template < class T >
inline
void dcm::HomogeneousTransform3d< T >::setDirectCoefficient( int32_t j, 
                                                             int32_t i,
                                                             T value,
                                                             bool update )
{

  assert( ( i >= 0 ) && ( i < 4 ) && ( j >= 0 ) && ( j < 4 ) );

  _directItems[ j ][ i ] = value;

  if ( update )
  {

    invert( _directItems, _inverseItems );

  }

}


template < class T >
inline
void dcm::HomogeneousTransform3d< T >::setInverseCoefficient( int32_t j, 
                                                              int32_t i,
                                                              T value,
                                                              bool update )
{

  assert( ( i >= 0 ) && ( i < 4 ) && ( j >= 0 ) && ( j < 4 ) );

  _inverseItems[ j ][ i ] = value;

  if ( update )
  {

    invert( _inverseItems, _directItems );

  }

}


template < class T >
inline
std::vector< T > dcm::HomogeneousTransform3d< T >::getDirectCoefficients() const
{

  std::vector< T > coef( 16 );

  coef[ 0 ] = _directItems[ 0 ][ 0 ];
  coef[ 1 ] = _directItems[ 0 ][ 1 ];
  coef[ 2 ] = _directItems[ 0 ][ 2 ];
  coef[ 3 ] = _directItems[ 0 ][ 3 ];

  coef[ 4 ] = _directItems[ 1 ][ 0 ];
  coef[ 5 ] = _directItems[ 1 ][ 1 ];
  coef[ 6 ] = _directItems[ 1 ][ 2 ];
  coef[ 7 ] = _directItems[ 1 ][ 3 ];

  coef[ 8 ] = _directItems[ 2 ][ 0 ];
  coef[ 9 ] = _directItems[ 2 ][ 1 ];
  coef[ 10 ] = _directItems[ 2 ][ 2 ];
  coef[ 11 ] = _directItems[ 2 ][ 3 ];

  coef[ 12 ] = _directItems[ 3 ][ 0 ];
  coef[ 13 ] = _directItems[ 3 ][ 1 ];
  coef[ 14 ] = _directItems[ 3 ][ 2 ];
  coef[ 15 ] = _directItems[ 3 ][ 3 ];

  return coef;

}


template < class T >
inline
dcm::HomogeneousTransform3d< T > 
dcm::HomogeneousTransform3d< T >::inverse() const
{

  dcm::HomogeneousTransform3d< T > t;
  int32_t i, j;
  
  for ( j = 0; j < 4; j++ )
  {
  
    for ( i = 0; i < 4; i++ )
    {
    
      t._directItems[ j ][ i ] = _inverseItems[ j ][ i ];
      t._inverseItems[ j ][ i ] = _directItems[ j ][ i ];
    
    }
  
  }
  
  return t;

}


template < class T >
inline
void dcm::HomogeneousTransform3d< T >::invert( T d[ 4 ][ 4 ], 
                                               T i[ 4 ][ 4 ] )
{

  T determinant = d[ 0 ][ 0 ] *
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


template < class T >
inline
dcm::HomogeneousTransform3d< T > operator * ( 
                                    const dcm::HomogeneousTransform3d< T >& t1,
                                    const dcm::HomogeneousTransform3d< T >& t2 )
{

  dcm::HomogeneousTransform3d< T > t;
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
  
  t.setDirectItems( m[ 0 ][ 0 ], m[ 0 ][ 1 ], m[ 0 ][ 2 ], m[ 0 ][ 3 ],
                    m[ 1 ][ 0 ], m[ 1 ][ 1 ], m[ 1 ][ 2 ], m[ 1 ][ 3 ],
                    m[ 2 ][ 0 ], m[ 2 ][ 1 ], m[ 2 ][ 2 ], m[ 2 ][ 3 ],
                    m[ 3 ][ 0 ], m[ 3 ][ 1 ], m[ 3 ][ 2 ], m[ 3 ][ 3 ] );
  
  return t;

}


std::ostream& std::operator << ( std::ostream& os, 
                                 const dcm::HomogeneousTransform3d< float >& t )
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


std::ostream& std::operator << ( 
                                std::ostream& os, 
                                const dcm::HomogeneousTransform3d< double >& t )
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


template class dcm::HomogeneousTransform3d< float >;
template class dcm::HomogeneousTransform3d< double >;

template dcm::HomogeneousTransform3d< float > 
operator * ( const dcm::HomogeneousTransform3d< float >& t1,
             const dcm::HomogeneousTransform3d< float >& t2 );
template dcm::HomogeneousTransform3d< double > 
operator * ( const dcm::HomogeneousTransform3d< double >& t1,
             const dcm::HomogeneousTransform3d< double >& t2 );
