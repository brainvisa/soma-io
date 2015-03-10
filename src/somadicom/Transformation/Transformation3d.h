#ifndef _Transformation3d_h_
#define _Transformation3d_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Utils/StdInt.h>
#endif

#include <vector>
#include <iostream>


namespace soma
{


class Transformation3d
{

  public:

    Transformation3d();
    Transformation3d( const Transformation3d& other );
    virtual ~Transformation3d();

    void setItems( double r00, double r01, double r02, double tx,
                   double r10, double r11, double r12, double ty,
                   double r20, double r21, double r22, double tz,
                   double sx, double sy, double sz, double w );

    virtual void getDirect( 
                    const double fromX, const double fromY, const double fromZ,
                    double& toX, double& toY, double& toZ ) const;
    virtual void getInverse( 
                          const double toX, const double toY, const double toZ,
                          double& fromX, double& fromY, double& fromZ ) const;

    double getDirectCoefficient( int32_t j, int32_t i ) const;
    double getInverseCoefficient( int32_t j, int32_t i ) const;

    void setDirectCoefficient( int32_t j, int32_t i, 
                               double value,
                               bool update = true );
    void setInverseCoefficient( int32_t j, int32_t i, 
                                double value,
                                bool update = true );

    std::vector< double > getDirectCoefficients() const;

    Transformation3d inverse() const;

  protected:

    double _direct[ 4 ][ 4 ];
    double _inverse[ 4 ][ 4 ];

    void invert( double d[ 4 ][ 4 ], double i[ 4 ][ 4 ] );

};


}


soma::Transformation3d operator * ( const soma::Transformation3d& t1,
                                    const soma::Transformation3d& t2 );


namespace std
{


ostream& operator << ( ostream& os, const soma::Transformation3d& t );


}


#endif
