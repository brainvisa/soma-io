#ifdef SOMA_IO_DICOM
#include <soma-io/Transformation/PatientOrientation.h>
#else
#include <Transformation/PatientOrientation.h>
#endif

#include <cmath>


soma::PatientOrientation::PatientOrientation()
                        : _onDiskSizeX( 0 ),
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
                        : _onDiskSizeX( other._onDiskSizeX ),
                          _onDiskSizeY( other._onDiskSizeY ),
                          _onDiskSizeZ( other._onDiskSizeZ ),
                          _onDiskResolution( other._onDiskResolution ),
                          _sizeX( other._sizeX ),
                          _sizeY( other._sizeY ),
                          _sizeZ( other._sizeZ ),
                          _resolution( other._resolution ),
                          _flipTransformation( other._flipTransformation ),
                          _axialTransformation( other._axialTransformation ),
                          _scaledTransformation( other._scaledTransformation ),
                          _dicomTransformation( other._dicomTransformation )
{
}


soma::PatientOrientation::~PatientOrientation()
{
}


void soma::PatientOrientation::set( const soma::Vector& rowVector,
                                    const soma::Vector& columnVector,
                                    const soma::Vector& origin,
                                    int32_t sizeX,
                                    int32_t sizeY,
                                    int32_t sizeZ,
                                    const soma::Vector& resolution )
{

  _onDiskSizeX = sizeX;
  _onDiskSizeY = sizeY;
  _onDiskSizeZ = sizeZ;
  _onDiskResolution = resolution;

  _flipTransformation = soma::FlipTransformation3d( rowVector, columnVector );
  _axialTransformation = soma::AxialTransformation3d( rowVector, columnVector );
  _dicomTransformation = soma::DicomTransformation3d( rowVector,
                                                      columnVector,
                                                      origin );

  double sX, sY, sZ;
  _axialTransformation.getDirect( double( _onDiskSizeX ),
                                  double( _onDiskSizeY ),
                                  double( _onDiskSizeZ ),
                                  sX,
                                  sY,
                                  sZ );
  _sizeX = int32_t( std::fabs( sX ) );
  _sizeY = int32_t( std::fabs( sY ) );
  _sizeZ = int32_t( std::fabs( sZ ) );

  soma::Vector t( ( sX < 0.0 ) ? -sX - 1.0 : 0.0,
                  ( sY < 0.0 ) ? -sY - 1.0 : 0.0,
                  ( sZ < 0.0 ) ? -sZ - 1.0 : 0.0 );
  _axialTransformation.setTranslation( t );

  _flipTransformation.getDirect( _onDiskResolution.x,
                                 _onDiskResolution.x,
                                 _onDiskResolution.z,
                                 _resolution.x,
                                 _resolution.y,
                                 _resolution.z );

  t.x *= _resolution.x;
  t.y *= _resolution.y;
  t.z *= _resolution.z;
  _scaledTransformation = soma::AxialTransformation3d( rowVector,
                                                       columnVector,
                                                       t );

}


void soma::PatientOrientation::setOrigin( const Vector& origin )
{

  _dicomTransformation.setTranslation( origin );

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
  outBoundingBox.setLowerX( int32_t( x ) );
  outBoundingBox.setLowerY( int32_t( y ) );
  outBoundingBox.setLowerZ( int32_t( z ) );
  outBoundingBox.setLowerT( boundingBox.getLowerT() );

  _flipTransformation.getDirect( double( boundingBox.getUpperX() ),
                                 double( boundingBox.getUpperY() ),
                                 double( boundingBox.getUpperZ() ),
                                 x, y, z );
  outBoundingBox.setUpperX( int32_t( x ) );
  outBoundingBox.setUpperY( int32_t( y ) );
  outBoundingBox.setUpperZ( int32_t( z ) );
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
  outBoundingBox.setLowerX( int32_t( x ) );
  outBoundingBox.setLowerY( int32_t( y ) );
  outBoundingBox.setLowerZ( int32_t( z ) );
  outBoundingBox.setLowerT( boundingBox.getLowerT() );

  _flipTransformation.getInverse( double( boundingBox.getUpperX() ),
                                  double( boundingBox.getUpperY() ),
                                  double( boundingBox.getUpperZ() ),
                                  x, y, z );
  outBoundingBox.setUpperX( int32_t( x ) );
  outBoundingBox.setUpperY( int32_t( y ) );
  outBoundingBox.setUpperZ( int32_t( z ) );
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

  soma::Transformation3d m = 
                         _dicomTransformation * _scaledTransformation.inverse();
  std::vector< double > c = m.getDirectCoefficients();

  // From Dicom to Nifti, only reverse X and Y coordinates
  m.setItems( -c[ 0 ],  -c[ 1 ],  -c[ 2 ],  -c[ 3 ],
              -c[ 4 ],  -c[ 5 ],  -c[ 6 ],  -c[ 7 ],
               c[ 8 ],   c[ 9 ],   c[ 10 ],  c[ 11 ],
               c[ 12 ],  c[ 13 ],  c[ 14 ],  c[ 15 ] );

  return m;

}
