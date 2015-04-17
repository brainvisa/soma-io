#ifndef _Vector3d_h_
#define _Vector3d_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Utils/StdInt.h>
#endif

#include <iostream>


namespace dcm
{


template < class T >
class Vector3d
{

  public:

    Vector3d();
    Vector3d( const Vector3d< T >& other );
    Vector3d( T theX, T theY, T theZ );

    Vector3d< T >& operator += ( const Vector3d< T >& other );
    Vector3d< T >& operator -= ( const Vector3d< T >& other );
    Vector3d< T >& operator *= ( T value );
    Vector3d< T >& operator /= ( T value );

    Vector3d< T > cross( const Vector3d< T >& other ) const;
    T dot( const Vector3d< T >& other );
    T getNorm();
    Vector3d< T >& normalize();
    Vector3d< T > mainDirection() const;
    Vector3d< T > absoluteCoordinates() const;

    T x;
    T y;
    T z;

};


}


template < class T >
dcm::Vector3d< T > operator + ( const dcm::Vector3d< T >& vec );
template < class T >
dcm::Vector3d< T > operator - ( const dcm::Vector3d< T >& vec );

template < class T >
dcm::Vector3d< T > operator + ( const dcm::Vector3d< T >& vec1, 
                                const dcm::Vector3d< T >& vec2 );
template < class T >
dcm::Vector3d< T > operator - ( const dcm::Vector3d< T >& vec1,
                                const dcm::Vector3d< T >& vec2 );
template < class T >
dcm::Vector3d< T > operator * ( const dcm::Vector3d< T >& vec, const T& val );
template < class T >
dcm::Vector3d< T > operator / ( const dcm::Vector3d< T >& vec, const T& val );


namespace std
{


ostream& operator << ( ostream& os, const dcm::Vector3d< int32_t >& vec );
ostream& operator << ( ostream& os, const dcm::Vector3d< float >& vec );
ostream& operator << ( ostream& os, const dcm::Vector3d< double >& vec );


}


#endif
