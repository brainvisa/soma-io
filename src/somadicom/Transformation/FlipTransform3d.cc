#ifdef SOMA_IO_DICOM
#include <soma-io/Transformation/FlipTransform3d.h>
#else
#include <Transformation/FlipTransform3d.h>
#endif


template < class T >
dcm::FlipTransform3d< T >::FlipTransform3d()
                         : dcm::AxialTransform3d< T >()
{
}


template < class T >
dcm::FlipTransform3d< T >::FlipTransform3d( 
                                        const dcm::Vector3d< T >& rowCosine,
                                        const dcm::Vector3d< T >& columnCosine )
                         : dcm::AxialTransform3d< T >()
{

  dcm::Vector3d< T > rowVector = 
                                rowCosine.mainDirection().absoluteCoordinates();
  dcm::Vector3d< T > columnVector = 
                             columnCosine.mainDirection().absoluteCoordinates();
  dcm::Vector3d< T > normalVector = 
                          rowVector.cross( columnVector ).absoluteCoordinates();

  this->initialize( normalVector );

  this->setDirectItems( rowVector.x, columnVector.x, normalVector.x, 0.0,
                        rowVector.y, columnVector.y, normalVector.y, 0.0,
                        rowVector.z, columnVector.z, normalVector.z, 0.0,
                        0.0,         0.0,            0.0,            1.0 );

}


template < class T >
dcm::FlipTransform3d< T >::FlipTransform3d( 
                                        const dcm::FlipTransform3d< T >& other )
                         : dcm::AxialTransform3d< T >( other )
{
}


template class dcm::FlipTransform3d< float >;
template class dcm::FlipTransform3d< double >;
