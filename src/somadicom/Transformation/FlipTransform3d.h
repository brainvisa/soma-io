#ifndef _FlipTransform_h_
#define _FlipTransform_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Transformation/AxialTransform3d.h>
#else
#include <Transformation/AxialTransform3d.h>
#endif


namespace dcm
{


template < class T >
class FlipTransform3d : public AxialTransform3d< T >
{

  public:

    FlipTransform3d();
    FlipTransform3d( const Vector3d< T >& rowCosine, 
                     const Vector3d< T >& columnCosine );
    FlipTransform3d( const FlipTransform3d< T >& other );

};


}


#endif
