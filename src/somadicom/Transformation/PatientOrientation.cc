#ifdef SOMA_IO_DICOM
#include <soma-io/Transformation/PatientOrientation.h>
#else
#include <Transformation/PatientOrientation.h>
#endif

#include <cmath>


soma::PatientOrientation::PatientOrientation()
                        : _axialAcquisition( false ),
                          _onDiskSizeX( 0 ),
                          _onDiskSizeY( 0 ),
                          _onDiskSizeZ( 0 ),
                          _onDiskResolution( 1.0, 1.0, 1.0 ),
                          _sizeX( 0 ),
                          _sizeY( 0 ),
                          _sizeZ( 0 ),
                          _resolution( 1.0, 1.0, 1.0 )
{
}


soma::PatientOrientation::PatientOrientation( 
                                         const soma::PatientOrientation& other )
                        : _axialAcquisition( other._axialAcquisition ),
                          _normalVector( other._normalVector ),
                          _onDiskSizeX( other._onDiskSizeX ),
                          _onDiskSizeY( other._onDiskSizeY ),
                          _onDiskSizeZ( other._onDiskSizeZ ),
                          _onDiskResolution( other._onDiskResolution ),
                          _sizeX( other._sizeX ),
                          _sizeY( other._sizeY ),
                          _sizeZ( other._sizeZ ),
                          _resolution( other._resolution ),
                          _axialTransformation( other._axialTransformation ),
                          _flipTransformation( other._flipTransformation ),
                          _dicomTransformation( other._dicomTransformation )
{
}


soma::PatientOrientation::~PatientOrientation()
{
}


void soma::PatientOrientation::set( const soma::Vector& rowVector,
                                    const soma::Vector& columnVector,
                                    const soma::Vector& normalVector,
                                    const soma::Vector& origin,
                                    int32_t sizeX,
                                    int32_t sizeY,
                                    int32_t sizeZ,
                                    const soma::Vector& resolution )
{

  _axialAcquisition = false;
  _onDiskSizeX = sizeX;
  _onDiskSizeY = sizeY;
  _onDiskSizeZ = sizeZ;
  _onDiskResolution = resolution;
  _normalVector = normalVector;

  _axialTransformation = soma::AxialTransformation3d( rowVector,
                                                      columnVector,
                                                      normalVector );
  _flipTransformation = _axialTransformation;
  _dicomTransformation = soma::DicomTransformation3d( rowVector,
                                                      columnVector,
                                                      normalVector );

  double sX, sY, sZ;
  _axialTransformation.getDirect( double( _onDiskSizeX ),
                                  double( _onDiskSizeY ),
                                  double( _onDiskSizeZ ),
                                  sX,
                                  sY,
                                  sZ );
  _sizeX = int32_t( std::fabs( sX ) + 0.5 );
  _sizeY = int32_t( std::fabs( sY ) + 0.5 );
  _sizeZ = int32_t( std::fabs( sZ ) + 0.5 );

  if ( std::fabs( _axialTransformation.getDirectCoefficient( 2, 2 ) ) > 0.0 )
  {

    _axialAcquisition = true;

  }

  _axialTransformation.getDirect( _onDiskResolution.x,
                                  _onDiskResolution.x,
                                  _onDiskResolution.z,
                                  _resolution.x,
                                  _resolution.y,
                                  _resolution.z );
  _resolution.x = std::fabs( _resolution.x );
  _resolution.y = std::fabs( _resolution.y );
  _resolution.z = std::fabs( _resolution.z );

  if ( rowVector.cross( columnVector ).dot( normalVector ) > 0.0 )
  {

    _axialTransformation.setTranslation( soma::Vector( double( _sizeX - 1 ),
                                                       double( _sizeY - 1 ),
                                                       double( _sizeZ - 1 ) ) );

  }

  setOrigin( origin );

}


void soma::PatientOrientation::setOrigin( const Vector& origin )
{

  soma::Vector position( origin );

  if ( _axialAcquisition )
  {

    position += _normalVector * double( _onDiskSizeZ * _onDiskResolution.z );

  }

  _dicomTransformation.setTranslation( position );

}


void soma::PatientOrientation::getOnDiskSize( int32_t& sizeX, 
                                              int32_t& sizeY, 
                                              int32_t& sizeZ )
{

  sizeX = _onDiskSizeX;
  sizeY = _onDiskSizeY;
  sizeZ = _onDiskSizeZ;

}


void soma::PatientOrientation::getSize( int32_t& sizeX, 
                                        int32_t& sizeY, 
                                        int32_t& sizeZ )
{

  sizeX = _sizeX;
  sizeY = _sizeY;
  sizeZ = _sizeZ;

}


void soma::PatientOrientation::getResolution( double& resolutionX,
                                              double& resolutionY,
                                              double& resolutionZ )
{

  resolutionX = _resolution.x;
  resolutionY = _resolution.y;
  resolutionZ = _resolution.z;

}


soma::BoundingBox< int32_t > 
soma::PatientOrientation::getDirectBoundingBox( 
                         const soma::BoundingBox< int32_t >& boundingBox ) const
{

  double x, y, z;
  soma::BoundingBox< int32_t > outBoundingBox;

  _flipTransformation.getDirect( double( boundingBox.getLowerX() ),
                                 double( boundingBox.getLowerY() ),
                                 double( boundingBox.getLowerZ() ),
                                 x, y, z );
  outBoundingBox.setLowerX( std::abs( int32_t( x + 0.5 ) ) );
  outBoundingBox.setLowerY( std::abs( int32_t( y + 0.5 ) ) );
  outBoundingBox.setLowerZ( std::abs( int32_t( z + 0.5 ) ) );
  outBoundingBox.setLowerT( boundingBox.getLowerT() );

  _flipTransformation.getDirect( double( boundingBox.getUpperX() ),
                                 double( boundingBox.getUpperY() ),
                                 double( boundingBox.getUpperZ() ),
                                 x, y, z );
  outBoundingBox.setUpperX( std::abs( int32_t( x + 0.5 ) ) );
  outBoundingBox.setUpperY( std::abs( int32_t( y + 0.5 ) ) );
  outBoundingBox.setUpperZ( std::abs( int32_t( z + 0.5 ) ) );
  outBoundingBox.setUpperT( boundingBox.getUpperT() );

  return outBoundingBox;

}


soma::BoundingBox< int32_t > 
soma::PatientOrientation::getInverseBoundingBox( 
                         const soma::BoundingBox< int32_t >& boundingBox ) const
{

  double x, y, z;
  soma::BoundingBox< int32_t > outBoundingBox;

  _flipTransformation.getInverse( double( boundingBox.getLowerX() ),
                                  double( boundingBox.getLowerY() ),
                                  double( boundingBox.getLowerZ() ),
                                  x, y, z );
  outBoundingBox.setLowerX( std::abs( int32_t( x + 0.5 ) ) );
  outBoundingBox.setLowerY( std::abs( int32_t( y + 0.5 ) ) );
  outBoundingBox.setLowerZ( std::abs( int32_t( z + 0.5 ) ) );
  outBoundingBox.setLowerT( boundingBox.getLowerT() );

  _flipTransformation.getInverse( double( boundingBox.getUpperX() ),
                                  double( boundingBox.getUpperY() ),
                                  double( boundingBox.getUpperZ() ),
                                  x, y, z );
  outBoundingBox.setUpperX( std::abs( int32_t( x + 0.5 ) ) );
  outBoundingBox.setUpperY( std::abs( int32_t( y + 0.5 ) ) );
  outBoundingBox.setUpperZ( std::abs( int32_t( z + 0.5 ) ) );
  outBoundingBox.setUpperT( boundingBox.getUpperT() );

  return outBoundingBox;

}


void soma::PatientOrientation::getDirect( const int32_t fromX, 
                                          const int32_t fromY,
                                          const int32_t fromZ,
                                          int32_t& toX, 
                                          int32_t& toY, 
                                          int32_t& toZ ) const
{

  double tX, tY, tZ;

  _axialTransformation.getDirect( double( fromX ),
                                  double( fromY ),
                                  double( fromZ ),
                                  tX,
                                  tY,
                                  tZ );

  toX = int32_t( tX + 0.5 );
  toY = int32_t( tY + 0.5 );
  toZ = int32_t( tZ + 0.5 );

}


void soma::PatientOrientation::getInverse( const int32_t toX, 
                                           const int32_t toY,
                                           const int32_t toZ, 
                                           int32_t& fromX, 
                                           int32_t& fromY, 
                                           int32_t& fromZ ) const
{

  double fX, fY, fZ;

  _axialTransformation.getInverse( double( toX ),
                                   double( toY ),
                                   double( toZ ),
                                   fX,
                                   fY,
                                   fZ );

  fromX = int32_t( fX + 0.5 );
  fromY = int32_t( fY + 0.5 );
  fromZ = int32_t( fZ + 0.5 );

}


const soma::Transformation3d& 
soma::PatientOrientation::getAxialTransformation() const
{

  return _axialTransformation;

}


soma::Transformation3d soma::PatientOrientation::getReferential() const
{

  return _dicomTransformation * _flipTransformation.inverse();

}
