#ifndef _AxialTransformation_h_
#define _AxialTransformation_h_


#include <Transformation/Transformation3d.h>
#include <Container/Vector.h>


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

  private:

    Vector changeToMainDirection( const Vector& vec ) const;

};


}


#endif
