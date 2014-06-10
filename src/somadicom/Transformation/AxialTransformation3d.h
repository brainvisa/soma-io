#ifndef _AxialTransformation_h_
#define _AxialTransformation_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Transformation/Transformation3d.h>
#include <soma-io/Container/Vector.h>


namespace soma
{


class Vector;


class AxialTransformation3d : public Transformation3d
{

  public:

    AxialTransformation3d();
    AxialTransformation3d( const Vector& rowCosine,
                           const Vector& columnCosine,
                           const Vector& normal );

};


}


#endif
