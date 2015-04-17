#ifdef SOMA_IO_DICOM
#include <soma-io/Transformation/AxialTransform3d.h>
#else
#include <Transformation/AxialTransform3d.h>
#endif

#include <cmath>


template < class T >
dcm::AxialTransform3d< T >::AxialTransform3d()
                          : dcm::HomogeneousTransform3d< T >(),
                            _directPointer( 0 ),
                            _inversePointer( 0 )
{
}


template < class T >
dcm::AxialTransform3d< T >::AxialTransform3d( 
                                         const dcm::Vector3d< T >& rowCosine,
                                         const dcm::Vector3d< T >& columnCosine,
                                         const dcm::Vector3d< T >& origin )
                          : dcm::HomogeneousTransform3d< T >()
{

  dcm::Vector3d< T > rowVector = rowCosine.mainDirection();
  dcm::Vector3d< T > columnVector = columnCosine.mainDirection();
  dcm::Vector3d< T > normalVector = rowVector.cross( columnVector );
  dcm::Vector3d< T > translation;

  if ( normalVector.z > 0.0 )
  {

    normalVector.z = -1.0;

  }
  else if ( columnVector.z < 0.0 )
  {

    columnVector.z = 1.0;

  }

  initialize( normalVector );

  this->setDirectItems( rowVector.x, columnVector.x, normalVector.x, 0.0,
                        rowVector.y, columnVector.y, normalVector.y, 0.0,
                        rowVector.z, columnVector.z, normalVector.z, 0.0,
                        0.0,         0.0,            0.0,            1.0 );

  getDirect( origin, translation );

  translation.x = ( translation.x < 0.0 ) ? -translation.x : 0.0;
  translation.y = ( translation.y < 0.0 ) ? -translation.y : 0.0;
  translation.z = ( translation.z < 0.0 ) ? -translation.z : 0.0;

  setTranslation( translation );

}


template < class T >
dcm::AxialTransform3d< T >::AxialTransform3d( 
                                       const dcm::AxialTransform3d< T >& other )
                          : dcm::HomogeneousTransform3d< T >( other ),
                            _directPointer( other._directPointer ),
                            _inversePointer( other._inversePointer )
{
}


template < class T >
inline
void dcm::AxialTransform3d< T >::setTranslation( 
                                         const dcm::Vector3d< T >& translation )
{

  this->setDirectCoefficient( 0, 3, translation.x, false );
  this->setDirectCoefficient( 1, 3, translation.y, false );
  this->setDirectCoefficient( 2, 3, translation.z );

}


template < class T >
inline
void dcm::AxialTransform3d< T >::getDirect( const dcm::Vector3d< T >& from, 
                                            dcm::Vector3d< T >& to ) const
{

  (this->*_directPointer)( from, to );

}


template < class T >
inline
void dcm::AxialTransform3d< T >::getInverse( const dcm::Vector3d< T >& to, 
                                             dcm::Vector3d< T >& from ) const
{

  (this->*_inversePointer)( to, from );

}


template < class T >
inline
void dcm::AxialTransform3d< T >::initialize(
                                        const dcm::Vector3d< T >& normalVector )
{

  if ( std::fabs( normalVector.z ) > 0.0 )
  {

    _directPointer = &dcm::AxialTransform3d< T >::getDirectAxial;
    _inversePointer = &dcm::AxialTransform3d< T >::getInverseAxial;

  }
  else if ( std::fabs( normalVector.y ) > 0.0 )
  {

    _directPointer = &dcm::AxialTransform3d< T >::getDirectCoronal;
    _inversePointer = &dcm::AxialTransform3d< T >::getInverseCoronal;

  }
  else
  {

    _directPointer = &dcm::AxialTransform3d< T >::getDirectSagittal;
    _inversePointer = &dcm::AxialTransform3d< T >::getInverseSagittal;

  }

}


template < class T >
inline
void dcm::AxialTransform3d< T >::getDirectAxial( const dcm::Vector3d< T >& from, 
                                                 dcm::Vector3d< T >& to ) const
{

   to.x = this->_directItems[ 0 ][ 0 ] * from.x + this->_directItems[ 0 ][ 3 ];
   to.y = this->_directItems[ 1 ][ 1 ] * from.y + this->_directItems[ 1 ][ 3 ];
   to.z = this->_directItems[ 2 ][ 2 ] * from.z + this->_directItems[ 2 ][ 3 ];

}


template < class T >
inline
void dcm::AxialTransform3d< T >::getDirectSagittal( 
                                                 const dcm::Vector3d< T >& from, 
                                                 dcm::Vector3d< T >& to ) const
{

  to.x = this->_directItems[ 0 ][ 2 ] * from.z + this->_directItems[ 0 ][ 3 ];
  to.y = this->_directItems[ 1 ][ 0 ] * from.x + this->_directItems[ 1 ][ 3 ];
  to.z = this->_directItems[ 2 ][ 1 ] * from.y + this->_directItems[ 2 ][ 3 ];

}


template < class T >
inline
void dcm::AxialTransform3d< T >::getDirectCoronal( 
                                                 const dcm::Vector3d< T >& from, 
                                                 dcm::Vector3d< T >& to ) const
{

  to.x = this->_directItems[ 0 ][ 0 ] * from.x + this->_directItems[ 0 ][ 3 ];
  to.y = this->_directItems[ 1 ][ 2 ] * from.z + this->_directItems[ 1 ][ 3 ];
  to.z = this->_directItems[ 2 ][ 1 ] * from.y + this->_directItems[ 2 ][ 3 ];

}


template < class T >
inline
void dcm::AxialTransform3d< T >::getInverseAxial( 
                                                const dcm::Vector3d< T >& to, 
                                                dcm::Vector3d< T >& from ) const
{

  from.x = this->_directItems[ 0 ][ 0 ] * ( to.x - 
                                            this->_directItems[ 0 ][ 3 ] );
  from.y = this->_directItems[ 1 ][ 1 ] * ( to.y -
                                            this->_directItems[ 1 ][ 3 ] );
  from.z = this->_directItems[ 2 ][ 2 ] * ( to.z -
                                            this->_directItems[ 2 ][ 3 ] );

}


template < class T >
inline
void dcm::AxialTransform3d< T >::getInverseSagittal( 
                                                const dcm::Vector3d< T >& to, 
                                                dcm::Vector3d< T >& from ) const
{

  from.x = this->_directItems[ 1 ][ 0 ] * ( to.y -
                                            this->_directItems[ 1 ][ 3 ] );
  from.y = this->_directItems[ 2 ][ 1 ] * ( to.z -
                                            this->_directItems[ 2 ][ 3 ] );
  from.z = this->_directItems[ 0 ][ 2 ] * ( to.x -
                                            this->_directItems[ 0 ][ 3 ] );

}


template < class T >
inline
void dcm::AxialTransform3d< T >::getInverseCoronal( 
                                                const dcm::Vector3d< T >& to, 
                                                dcm::Vector3d< T >& from ) const
{

  from.x = this->_directItems[ 0 ][ 0 ] * ( to.x -
                                            this->_directItems[ 0 ][ 3 ] );
  from.y = this->_directItems[ 2 ][ 1 ] * ( to.z -
                                            this->_directItems[ 2 ][ 3 ] );
  from.z = this->_directItems[ 1 ][ 2 ] * ( to.y -
                                            this->_directItems[ 1 ][ 3 ] );

}


template class dcm::AxialTransform3d< float >;
template class dcm::AxialTransform3d< double >;
