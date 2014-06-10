#ifndef _DicomTransformation3d_h_
#define _DicomTransformation3d_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Transformation/Transformation3d.h>


namespace soma
{


class Vector;


class DicomTransformation3d : public Transformation3d
{

  public:

    DicomTransformation3d();
    DicomTransformation3d( const Vector& rowCosine,
                           const Vector& columnCosine,
                           const Vector& normal,
                           const Vector& origin );

    void setTranslation( const Vector& translation );

};


}


#endif
