#ifdef SOMA_IO_DICOM
#include <soma-io/Transformation/PatientOrientation.h>
#else
#include <Transformation/PatientOrientation.h>
#endif


dcm::PatientOrientation::PatientOrientation()
{
}


dcm::PatientOrientation::PatientOrientation( 
                                          const dcm::PatientOrientation& other )
                       : _onDiskSize( other._onDiskSize ),
                         _onDiskResolution( other._onDiskResolution ),
                         _size( other._size ),
                         _resolution( other._resolution ),
                         _flipTransform( other._flipTransform ),
                         _axialTransform( other._axialTransform ),
                         _scaledTransform( other._scaledTransform ),
                         _dicomTransform( other._dicomTransform )
{
}


dcm::PatientOrientation::~PatientOrientation()
{
}


void dcm::PatientOrientation::set( const dcm::Vector3d< double >& rowVector,
                                   const dcm::Vector3d< double >& columnVector,
                                   const dcm::Vector3d< double >& origin,
                                   int32_t sizeX,
                                   int32_t sizeY,
                                   int32_t sizeZ,
                                   const dcm::Vector3d< double >& resolution )
{

  dcm::Vector3d< double > translation( sizeX - 1, sizeY - 1, sizeZ - 1 );

  _onDiskSize.x = sizeX;
  _onDiskSize.y = sizeY;
  _onDiskSize.z = sizeZ;
  _onDiskResolution = resolution;

  _flipTransform = dcm::FlipTransform3d< double >( rowVector, columnVector );
  _flipTransform.getFullIntegerDirect( _onDiskSize, _size );
  _flipTransform.getDirect( _onDiskResolution, _resolution );

  _dicomTransform = dcm::DicomTransform3d< double >( rowVector,
                                                     columnVector,
                                                     origin );

  _axialTransform = dcm::AxialTransform3d< double >( rowVector, 
                                                     columnVector,
                                                     translation );

  translation.x *= resolution.x;
  translation.y *= resolution.y;
  translation.z *= resolution.z;
  _scaledTransform = dcm::AxialTransform3d< double >( rowVector,
                                                      columnVector,
                                                      translation );

}


void dcm::PatientOrientation::setOrigin( const Vector3d< double >& origin )
{

  _dicomTransform.setTranslation( origin );

}


dcm::Vector3d< int32_t > dcm::PatientOrientation::getOnDiskSize() const
{

  return _onDiskSize;

}


dcm::Vector3d< int32_t > dcm::PatientOrientation::getSize() const
{

  return _size;

}


dcm::Vector3d< double > dcm::PatientOrientation::getResolution() const
{

  return _resolution;

}


dcm::BoundingBox< int32_t > 
dcm::PatientOrientation::getDirectBoundingBox( 
                          const dcm::BoundingBox< int32_t >& boundingBox ) const
{

  dcm::BoundingBox< int32_t > outBoundingBox;
  dcm::Vector3d< double > lowerBB( double( boundingBox.getLowerX() ),
                                   double( boundingBox.getLowerY() ),
                                   double( boundingBox.getLowerZ() ) );
  dcm::Vector3d< double > upperBB( double( boundingBox.getUpperX() ),
                                   double( boundingBox.getUpperY() ),
                                   double( boundingBox.getUpperZ() ) );
  dcm::Vector3d< double > to;

  _flipTransform.getDirect( lowerBB, to );
  outBoundingBox.setLowerX( int32_t( to.x ) );
  outBoundingBox.setLowerY( int32_t( to.y ) );
  outBoundingBox.setLowerZ( int32_t( to.z ) );
  outBoundingBox.setLowerT( boundingBox.getLowerT() );

  _flipTransform.getDirect( upperBB, to );
  outBoundingBox.setUpperX( int32_t( to.x ) );
  outBoundingBox.setUpperY( int32_t( to.y ) );
  outBoundingBox.setUpperZ( int32_t( to.z ) );
  outBoundingBox.setUpperT( boundingBox.getUpperT() );

  return outBoundingBox;

}


dcm::BoundingBox< int32_t > 
dcm::PatientOrientation::getInverseBoundingBox( 
                          const dcm::BoundingBox< int32_t >& boundingBox ) const
{

  dcm::BoundingBox< int32_t > outBoundingBox;
  dcm::Vector3d< double > lowerBB( double( boundingBox.getLowerX() ),
                                   double( boundingBox.getLowerY() ),
                                   double( boundingBox.getLowerZ() ) );
  dcm::Vector3d< double > upperBB( double( boundingBox.getUpperX() ),
                                   double( boundingBox.getUpperY() ),
                                   double( boundingBox.getUpperZ() ) );
  dcm::Vector3d< double > from;

  _flipTransform.getInverse( lowerBB, from );
  outBoundingBox.setLowerX( int32_t( from.x ) );
  outBoundingBox.setLowerY( int32_t( from.y ) );
  outBoundingBox.setLowerZ( int32_t( from.z ) );
  outBoundingBox.setLowerT( boundingBox.getLowerT() );

  _flipTransform.getInverse( upperBB, from );
  outBoundingBox.setUpperX( int32_t( from.x ) );
  outBoundingBox.setUpperY( int32_t( from.y ) );
  outBoundingBox.setUpperZ( int32_t( from.z ) );
  outBoundingBox.setUpperT( boundingBox.getUpperT() );

  return outBoundingBox;

}


dcm::Vector3d< int32_t > dcm::PatientOrientation::getDirect( int32_t x, 
                                                             int32_t y, 
                                                             int32_t z ) const
{

  dcm::Vector3d< double > from( x, y, z );
  dcm::Vector3d< double > to;

  _axialTransform.getDirect( from, to );

  return dcm::Vector3d< int32_t >( int32_t( to.x + 0.5 ),
                                   int32_t( to.y + 0.5 ),
                                   int32_t( to.z + 0.5 ) );

}


dcm::Vector3d< int32_t > dcm::PatientOrientation::getInverse( int32_t x, 
                                                              int32_t y,
                                                              int32_t z ) const
{

  dcm::Vector3d< double > to( x, y, z );
  dcm::Vector3d< double > from;

  _axialTransform.getInverse( to, from );

  return dcm::Vector3d< int32_t >( int32_t( from.x + 0.5 ),
                                   int32_t( from.y + 0.5 ),
                                   int32_t( from.z + 0.5 ) );

}


const dcm::HomogeneousTransform3d< double >& 
dcm::PatientOrientation::getAxialTransformation() const
{

  return _axialTransform;

}


dcm::HomogeneousTransform3d< double > 
dcm::PatientOrientation::getReferential() const
{

  dcm::HomogeneousTransform3d< double > m = 
                                   _dicomTransform * _scaledTransform.inverse();

  std::vector< double > c = m.getDirectCoefficients();

  // Changing from Dicom to Nifti referential only requires X and Y inversions
  m.setDirectItems( -c[ 0 ],  -c[ 1 ],  -c[ 2 ],  -c[ 3 ],
                    -c[ 4 ],  -c[ 5 ],  -c[ 6 ],  -c[ 7 ],
                     c[ 8 ],   c[ 9 ],   c[ 10 ],  c[ 11 ],
                     c[ 12 ],  c[ 13 ],  c[ 14 ],  c[ 15 ] );

  return m;

}
