#ifndef _FlipTransformation_h_
#define _FlipTransformation_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Transformation/AxialTransformation3d.h>
#else
#include <Transformation/AxialTransformation3d.h>
#endif


namespace soma
{


class FlipTransformation3d : public AxialTransformation3d
{

  public:

    FlipTransformation3d();
    FlipTransformation3d( const Vector& rowCosine, const Vector& columnCosine );
    FlipTransformation3d( const FlipTransformation3d& other );

};


}


#endif
