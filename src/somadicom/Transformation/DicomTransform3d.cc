#ifdef SOMA_IO_DICOM
#include <soma-io/Transformation/DicomTransform3d.h>
#else
#include <Transformation/DicomTransform3d.h>
#endif


template < class T >
dcm::DicomTransform3d< T >::DicomTransform3d()
                          : dcm::HomogeneousTransform3d< T >()
{
}


template < class T >
dcm::DicomTransform3d< T >::DicomTransform3d( 
                                         const dcm::Vector3d< T >& rowCosine,
                                         const dcm::Vector3d< T >& columnCosine,
                                         const dcm::Vector3d< T >& origin )
                          : dcm::HomogeneousTransform3d< T >()
{

  dcm::Vector3d< T > normalVector = rowCosine.cross( columnCosine );

  this->setDirectItems( rowCosine.x, columnCosine.x, normalVector.x,  origin.x,
                        rowCosine.y, columnCosine.y, normalVector.y,  origin.y,
                        rowCosine.z, columnCosine.z, normalVector.z,  origin.z,
                        0.0,         0.0,            0.0,             1.0 );

}


template < class T >
dcm::DicomTransform3d< T >::DicomTransform3d( 
                                       const dcm::DicomTransform3d< T >& other )
                          : dcm::HomogeneousTransform3d< T >( other )
{
}


template < class T >
inline
void dcm::DicomTransform3d< T >::setTranslation( 
                                         const dcm::Vector3d< T >& translation )
{

  this->setDirectCoefficient( 0, 3, translation.x, false );
  this->setDirectCoefficient( 1, 3, translation.y, false );
  this->setDirectCoefficient( 2, 3, translation.z );

}


template class dcm::DicomTransform3d< float >;
template class dcm::DicomTransform3d< double >;
