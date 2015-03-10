#ifdef SOMA_IO_DICOM
#include <soma-io/Transformation/AxialTransformation3d.h>
#else
#include <Transformation/AxialTransformation3d.h>
#endif

#include <cmath>


soma::AxialTransformation3d::AxialTransformation3d()
                           : soma::Transformation3d(),
                             _directPointer( 0 ),
                             _inversePointer( 0 )
{
}


soma::AxialTransformation3d::AxialTransformation3d( 
                                               const soma::Vector& rowCosine,
                                               const soma::Vector& columnCosine,
                                               const soma::Vector& origin )
                           : soma::Transformation3d()
{

  soma::Vector rowVec = rowCosine.mainDirection();
  soma::Vector columnVec = columnCosine.mainDirection();
  soma::Vector normalVec = rowVec.cross( columnVec );

  if ( normalVec.z > 0.0 )
  {

    normalVec.z = -1.0;

  }
  else if ( columnVec.z < 0.0 )
  {

    columnVec.z = 1.0;

  }

  initialize( normalVec );

  setItems( rowVec.x, columnVec.x, normalVec.x, origin.x,
            rowVec.y, columnVec.y, normalVec.y, origin.y,
            rowVec.z, columnVec.z, normalVec.z, origin.z,
            0.0,      0.0,         0.0,         1.0 );

}


soma::AxialTransformation3d::AxialTransformation3d( 
                                      const soma::AxialTransformation3d& other )
                           : soma::Transformation3d( other ),
                             _directPointer( other._directPointer ),
                             _inversePointer( other._inversePointer )
{
}


void soma::AxialTransformation3d::setTranslation( 
                                               const soma::Vector& translation )
{

  _direct[ 0 ][ 3 ] = translation.x;
  _direct[ 1 ][ 3 ] = translation.y;
  _direct[ 2 ][ 3 ] = translation.z;

  invert( _direct, _inverse );

}


void soma::AxialTransformation3d::getDirect( 
                     const double fromX, const double fromY, const double fromZ,
                     double& toX, double& toY, double& toZ ) const
{

  (this->*_directPointer)( fromX, fromY, fromZ, toX, toY, toZ );

}


void soma::AxialTransformation3d::getInverse( 
                           const double toX, const double toY, const double toZ,
                           double& fromX, double& fromY, double& fromZ ) const
{

  (this->*_inversePointer)( toX, toY, toZ, fromX, fromY, fromZ );

}


void soma::AxialTransformation3d::initialize( const soma::Vector& normalVec )
{

  if ( std::fabs( normalVec.z ) > 0.0 )
  {

    _directPointer = &soma::AxialTransformation3d::getDirectAxial;
    _inversePointer = &soma::AxialTransformation3d::getInverseAxial;

  }
  else if ( std::fabs( normalVec.y ) > 0.0 )
  {

    _directPointer = &soma::AxialTransformation3d::getDirectCoronal;
    _inversePointer = &soma::AxialTransformation3d::getInverseCoronal;

  }
  else
  {

    _directPointer = &soma::AxialTransformation3d::getDirectSagittal;
    _inversePointer = &soma::AxialTransformation3d::getInverseSagittal;

  }

}


void soma::AxialTransformation3d::getDirectAxial( 
                     const double fromX, const double fromY, const double fromZ,
                     double& toX, double& toY, double& toZ ) const
{

   toX = _direct[ 0 ][ 0 ] * fromX + _direct[ 0 ][ 3 ];
   toY = _direct[ 1 ][ 1 ] * fromY + _direct[ 1 ][ 3 ];
   toZ = _direct[ 2 ][ 2 ] * fromZ + _direct[ 2 ][ 3 ];

}


void soma::AxialTransformation3d::getDirectSagittal( 
                     const double fromX, const double fromY, const double fromZ,
                     double& toX, double& toY, double& toZ ) const
{

  toX = _direct[ 0 ][ 2 ] * fromZ + _direct[ 0 ][ 3 ];
  toY = _direct[ 1 ][ 0 ] * fromX + _direct[ 1 ][ 3 ];
  toZ = _direct[ 2 ][ 1 ] * fromY + _direct[ 2 ][ 3 ];

}


void soma::AxialTransformation3d::getDirectCoronal( 
                     const double fromX, const double fromY, const double fromZ,
                     double& toX, double& toY, double& toZ ) const
{

  toX = _direct[ 0 ][ 0 ] * fromX + _direct[ 0 ][ 3 ];
  toY = _direct[ 1 ][ 2 ] * fromZ + _direct[ 1 ][ 3 ];
  toZ = _direct[ 2 ][ 1 ] * fromY + _direct[ 2 ][ 3 ];

}


void soma::AxialTransformation3d::getInverseAxial( 
                           const double toX, const double toY, const double toZ,
                           double& fromX, double& fromY, double& fromZ ) const
{

  fromX = _direct[ 0 ][ 0 ] * ( toX - _direct[ 0 ][ 3 ] );
  fromY = _direct[ 1 ][ 1 ] * ( toY - _direct[ 1 ][ 3 ] );
  fromZ = _direct[ 2 ][ 2 ] * ( toZ - _direct[ 2 ][ 3 ] );

}


void soma::AxialTransformation3d::getInverseSagittal( 
                           const double toX, const double toY, const double toZ,
                           double& fromX, double& fromY, double& fromZ ) const
{

  fromX = _direct[ 1 ][ 0 ] * ( toY - _direct[ 1 ][ 3 ] );
  fromY = _direct[ 2 ][ 1 ] * ( toZ - _direct[ 2 ][ 3 ] );
  fromZ = _direct[ 0 ][ 2 ] * ( toX - _direct[ 0 ][ 3 ] );

}


void soma::AxialTransformation3d::getInverseCoronal( 
                           const double toX, const double toY, const double toZ,
                           double& fromX, double& fromY, double& fromZ ) const
{

  fromX = _direct[ 0 ][ 0 ] * ( toX - _direct[ 0 ][ 3 ] );
  fromY = _direct[ 2 ][ 1 ] * ( toZ - _direct[ 2 ][ 3 ] );
  fromZ = _direct[ 1 ][ 2 ] * ( toY - _direct[ 1 ][ 3 ] );

}
