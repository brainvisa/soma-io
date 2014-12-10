#ifndef _AxialTransformation_h_
#define _AxialTransformation_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Transformation/Transformation3d.h>
#include <soma-io/Container/Vector.h>


namespace soma
{


class AxialTransformation3d : public Transformation3d
{

  public:

    AxialTransformation3d();
    AxialTransformation3d( const Vector& rowCosine,
                           const Vector& columnCosine,
                           const Vector& normal,
                           const Vector& origin = Vector( 0.0, 0.0, 0.0 )  );
    AxialTransformation3d( const AxialTransformation3d& other );

    void setTranslation( const Vector& translation );

    void getDirect( const double fromX, const double fromY, const double fromZ,
                    double& toX, double& toY, double& toZ );
    void getInverse( const double toX, const double toY, const double toZ,
                     double& fromX, double& fromY, double& fromZ );

};


}


#endif
