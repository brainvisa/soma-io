#include <soma-io/Transformation/PatientOrientation.h>

#include <cmath>


soma::PatientOrientation::PatientOrientation()
                        : m_axialAcquisition( false ),
                          m_onDiskSizeX( 0 ),
                          m_onDiskSizeY( 0 ),
                          m_onDiskSizeZ( 0 ),
                          m_onDiskResolution( 1.0, 1.0, 1.0 ),
                          m_sizeX( 0 ),
                          m_sizeY( 0 ),
                          m_sizeZ( 0 ),
                          m_resolution( 1.0, 1.0, 1.0 )
{
}


soma::PatientOrientation::PatientOrientation( 
                                         const soma::PatientOrientation& other )
                        : m_axialAcquisition( other.m_axialAcquisition ),
                          m_onDiskSizeX( other.m_onDiskSizeX ),
                          m_onDiskSizeY( other.m_onDiskSizeY ),
                          m_onDiskSizeZ( other.m_onDiskSizeZ ),
                          m_onDiskResolution( other.m_onDiskResolution ),
                          m_sizeX( other.m_sizeX ),
                          m_sizeY( other.m_sizeY ),
                          m_sizeZ( other.m_sizeZ ),
                          m_resolution( other.m_resolution ),
                          m_normalVector( other.m_normalVector ),
                          m_axialTransformation( other.m_axialTransformation ),
                          m_dicomTransformation( other.m_dicomTransformation )
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

  m_axialAcquisition = false;
  m_onDiskSizeX = sizeX;
  m_onDiskSizeY = sizeY;
  m_onDiskSizeZ = sizeZ;
  m_onDiskResolution = resolution;
  m_normalVector = normalVector;

  m_axialTransformation = soma::AxialTransformation3d( rowVector,
                                                       columnVector,
                                                       normalVector );
  m_dicomTransformation = soma::DicomTransformation3d( rowVector,
                                                       columnVector,
                                                       normalVector );

  double sX, sY, sZ;
  m_axialTransformation.getDirect( double( m_onDiskSizeX ),
                                   double( m_onDiskSizeY ),
                                   double( m_onDiskSizeZ ),
                                   sX,
                                   sY,
                                   sZ );
  m_sizeX = int32_t( std::fabs( sX ) + 0.5 );
  m_sizeY = int32_t( std::fabs( sY ) + 0.5 );
  m_sizeZ = int32_t( std::fabs( sZ ) + 0.5 );

  if ( std::fabs( m_axialTransformation.getDirectCoefficient( 2, 2 ) ) > 0.0 )
  {

    m_axialAcquisition = true;

  }

  m_axialTransformation.getDirect( m_onDiskResolution.x,
                                   m_onDiskResolution.y,
                                   m_onDiskResolution.z,
                                   m_resolution.x,
                                   m_resolution.y,
                                   m_resolution.z );
  m_resolution.x = std::fabs( m_resolution.x );
  m_resolution.y = std::fabs( m_resolution.y );
  m_resolution.z = std::fabs( m_resolution.z );

  if ( rowVector.cross( columnVector ).dot( normalVector ) > 0.0 )
  {

    m_axialTransformation.setTranslation( soma::Vector( 
                                                      double( m_sizeX - 1 ),
                                                      double( m_sizeY - 1 ),
                                                      double( m_sizeZ - 1 ) ) );

  }

  setOrigin( origin );

}


void soma::PatientOrientation::setOrigin( const Vector& origin )
{

  soma::Vector position( origin );

  if ( m_axialAcquisition )
  {

    position += m_normalVector * double( m_onDiskSizeZ * m_onDiskResolution.z );

  }

  m_dicomTransformation.setTranslation( position );

}


void soma::PatientOrientation::getOnDiskSize( int32_t& sizeX, 
                                              int32_t& sizeY, 
                                              int32_t& sizeZ )
{

  sizeX = m_onDiskSizeX;
  sizeY = m_onDiskSizeY;
  sizeZ = m_onDiskSizeZ;

}


void soma::PatientOrientation::getSize( int32_t& sizeX, 
                                        int32_t& sizeY, 
                                        int32_t& sizeZ )
{

  sizeX = m_sizeX;
  sizeY = m_sizeY;
  sizeZ = m_sizeZ;

}


void soma::PatientOrientation::getResolution( double& resolutionX,
                                              double& resolutionY,
                                              double& resolutionZ )
{

  resolutionX = m_resolution.x;
  resolutionY = m_resolution.y;
  resolutionZ = m_resolution.z;

}


void soma::PatientOrientation::getDirect( const int32_t fromX, 
                                          const int32_t fromY,
                                          const int32_t fromZ,
                                          int32_t& toX, 
                                          int32_t& toY, 
                                          int32_t& toZ )
{

  double tX, tY, tZ;

  m_axialTransformation.getDirect( double( fromX ),
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
                                           int32_t& fromZ )
{

  double fX, fY, fZ;

  m_axialTransformation.getInverse( double( toX ),
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

  return m_axialTransformation;

}


soma::Transformation3d soma::PatientOrientation::getReferential() const
{

  return m_dicomTransformation * m_axialTransformation.inverse();

}
