#ifndef _DicomTransform3d_h_
#define _DicomTransform3d_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Transformation/HomogeneousTransform3d.h>
#else
#include <Transformation/HomogeneousTransform3d.h>
#endif


namespace dcm
{


template < class T >
class DicomTransform3d : public HomogeneousTransform3d< T >
{

  public:

    DicomTransform3d();
    DicomTransform3d( const Vector3d< T >& rowCosine,
                      const Vector3d< T >& columnCosine,
                      const Vector3d< T >& origin = Vector3d< T >( 0, 0, 0 ) );
    DicomTransform3d( const DicomTransform3d< T >& other );

    void setTranslation( const Vector3d< T >& translation );

};


}


#endif
