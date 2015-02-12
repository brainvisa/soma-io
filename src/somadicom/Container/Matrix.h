#ifndef _Matrix_h_
#define _Matrix_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Container/Vector.h>
#else
#include <Container/Vector.h>
#endif


namespace soma
{


class Matrix
{

  public:

    Matrix();
    Matrix( const Matrix& other );
    Matrix( double m00, double m01, double m02,
            double m10, double m11, double m12,
            double m20, double m21, double m22 );
    Matrix( const Vector& rowVec, const Vector& colVec, const Vector& normVec );

    Matrix inverse();

    double m[ 3 ][ 3 ];

};


}


soma::Matrix operator * ( const soma::Matrix& mat1, const soma::Matrix& mat2 );
soma::Vector operator * ( const soma::Matrix& mat, const soma::Vector& vec );


namespace std
{


ostream& operator << ( ostream& os, const soma::Matrix& m );


}


#endif
