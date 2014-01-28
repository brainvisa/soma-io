#include <soma-io/Transformation/PatientOrientation.h>
#include <soma-io/Container/Vector.h>

#include <cmath>


soma::PatientOrientation::PatientOrientation()
                        : m_axialAcquisition( false ),
                          m_onDiskSizeX( 0 ),
                          m_onDiskSizeY( 0 ),
                          m_onDiskSizeZ( 0 ),
                          m_onDiskResolutionX( 1.0 ),
                          m_onDiskResolutionY( 1.0 ),
                          m_onDiskResolutionZ( 1.0 ),
                          m_sizeX( 0 ),
                          m_sizeY( 0 ),
                          m_sizeZ( 0 ),
                          m_resolutionX( 1.0 ),
                          m_resolutionY( 1.0 ),
                          m_resolutionZ( 1.0 )
{
}


soma::PatientOrientation::PatientOrientation( 
                                         const soma::PatientOrientation& other )
                        : m_axialAcquisition( other.m_axialAcquisition ),
                          m_onDiskSizeX( other.m_onDiskSizeX ),
                          m_onDiskSizeY( other.m_onDiskSizeY ),
                          m_onDiskSizeZ( other.m_onDiskSizeZ ),
                          m_onDiskResolutionX( other.m_onDiskResolutionX ),
                          m_onDiskResolutionY( other.m_onDiskResolutionY ),
                          m_onDiskResolutionZ( other.m_onDiskResolutionZ ),
                          m_sizeX( other.m_sizeX ),
                          m_sizeY( other.m_sizeY ),
                          m_sizeZ( other.m_sizeZ ),
                          m_resolutionX( other.m_resolutionX ),
                          m_resolutionY( other.m_resolutionY ),
                          m_resolutionZ( other.m_resolutionZ ),
                          m_normalVector( other.m_normalVector ),
                          m_axialTransformation( other.m_axialTransformation ),
                          m_dicomTransformation( other.m_dicomTransformation )
{
}


void soma::PatientOrientation::set( const soma::Vector& rowVector,
                                    const soma::Vector& columnVector,
                                    const soma::Vector& normalVector,
                                    const soma::Vector& origin,
                                    int32_t sizeX,
                                    int32_t sizeY,
                                    int32_t sizeZ,
                                    double pixelSpacingX,
                                    double pixelSpacingY,
                                    double pixelSpacingZ )
{

  m_axialAcquisition = false;
  m_onDiskSizeX = sizeX;
  m_onDiskSizeY = sizeY;
  m_onDiskSizeZ = sizeZ;
  m_onDiskResolutionX = pixelSpacingX;
  m_onDiskResolutionY = pixelSpacingY;
  m_onDiskResolutionZ = pixelSpacingZ;
  m_normalVector = normalVector;

  m_axialTransformation = soma::AxialTransformation3d( rowVector,
                                                       columnVector,
                                                       normalVector );
  m_dicomTransformation = soma::DicomTransformation3d( rowVector,
                                                       columnVector,
                                                       normalVector,
                                                       soma::Vector( 0.0, 
                                                                     0.0, 
                                                                     0.0 ) );

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

  if ( std::fabs( m_axialTransformation.getDirectCoefficient( 2, 2 ) ) > 1e-12 )
  {

    m_axialAcquisition = true;

  }

  m_axialTransformation.getDirect( m_onDiskResolutionX,
                                   m_onDiskResolutionY,
                                   m_onDiskResolutionZ,
                                   m_resolutionX,
                                   m_resolutionY,
                                   m_resolutionZ );
  m_resolutionX = std::fabs( m_resolutionX );
  m_resolutionY = std::fabs( m_resolutionY );
  m_resolutionZ = std::fabs( m_resolutionZ );

  setOrigin( origin );

}


soma::PatientOrientation::~PatientOrientation()
{
}


void soma::PatientOrientation::setOrigin( const Vector& origin )
{

  soma::Vector position( origin );

  if ( m_axialAcquisition )
  {

    position += m_normalVector * double( m_onDiskSizeZ * m_onDiskResolutionZ );

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

  resolutionX = m_resolutionX;
  resolutionY = m_resolutionY;
  resolutionZ = m_resolutionZ;

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

  if ( m_axialAcquisition )
  {

    toZ += m_sizeZ - 1;

  }

}


void soma::PatientOrientation::getInverse( const int32_t toX, 
                                           const int32_t toY,
                                           const int32_t toZ, 
                                           int32_t& fromX, 
                                           int32_t& fromY, 
                                           int32_t& fromZ )
{

  int32_t z = toZ;
  double fX, fY, fZ;

  if ( m_axialAcquisition )
  {

    z -= m_sizeZ - 1;

  }

  m_axialTransformation.getInverse( double( toX ),
                                    double( toY ),
                                    double( z ),
                                    fX,
                                    fY,
                                    fZ );

  fromX = int32_t( fX + 0.5 );
  fromY = int32_t( fY + 0.5 );
  fromZ = int32_t( fZ + 0.5 );

}


soma::Transformation3d soma::PatientOrientation::getReferential() const
{

  return m_dicomTransformation * m_axialTransformation.inverse();

}
