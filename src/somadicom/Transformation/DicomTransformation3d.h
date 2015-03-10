#ifndef _DicomTransformation3d_h_
#define _DicomTransformation3d_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Transformation/Transformation3d.h>
#include <soma-io/Container/Vector.h>
#else
#include <Transformation/Transformation3d.h>
#include <Container/Vector.h>
#endif


namespace soma
{


class DicomTransformation3d : public Transformation3d
{

  public:

    DicomTransformation3d();
    DicomTransformation3d( const Vector& rowCosine,
                           const Vector& columnCosine,
                           const Vector& origin = Vector( 0.0, 0.0, 0.0 ) );
    DicomTransformation3d( const DicomTransformation3d& other );

    void setTranslation( const Vector& translation );

};


}


#endif
