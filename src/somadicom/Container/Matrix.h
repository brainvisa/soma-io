#ifndef _Matrix_h_
#define _Matrix_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Container/Vector3d.h>
#else
#include <Container/Vector3d.h>
#endif


namespace dcm
{


template < class T >
class Matrix
{

  public:

    Matrix();
    Matrix( const Matrix& other );
    Matrix( T m00, T m01, T m02,
            T m10, T m11, T m12,
            T m20, T m21, T m22 );
    Matrix( const Vector3d< T >& rowVec, 
            const Vector3d< T >& colVec, 
            const Vector3d< T >& normVec );

    Matrix inverse();

    T m[ 3 ][ 3 ];

};


}


template < class T >
dcm::Matrix< T > operator * ( const dcm::Matrix< T >& mat1, 
                              const dcm::Matrix< T >& mat2 );
template < class T >
dcm::Vector3d< T > operator * ( const dcm::Matrix< T >& mat, 
                                const dcm::Vector3d< T >& vec );


namespace std
{


ostream& operator << ( ostream& os, const dcm::Matrix< float >& m );
ostream& operator << ( ostream& os, const dcm::Matrix< double >& m );


}


#endif
