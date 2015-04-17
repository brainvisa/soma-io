#ifndef _HomogeneousTransform3d_h_
#define _HomogeneousTransform3d_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Container/Vector3d.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Container/Vector3d.h>
#include <Utils/StdInt.h>
#endif

#include <vector>
#include <iostream>


namespace dcm
{


template < class T >
class HomogeneousTransform3d
{

  public:

    HomogeneousTransform3d();
    HomogeneousTransform3d( const HomogeneousTransform3d& other );
    virtual ~HomogeneousTransform3d();

    void setDirectItems( T r00, T r01, T r02, T tx,
                         T r10, T r11, T r12, T ty,
                         T r20, T r21, T r22, T tz,
                         T sx,  T sy,  T sz,  T w );

    virtual void getDirect( const Vector3d< T >& from, 
                            Vector3d< T >& to ) const;
    virtual void getInverse( const Vector3d< T >& to, 
                             Vector3d< T >& from ) const;
    virtual void getFullIntegerDirect( const Vector3d< int32_t >& from, 
                                       Vector3d< int32_t >& to ) const;
    virtual void getFullIntegerInverse( const Vector3d< int32_t >& to, 
                                        Vector3d< int32_t >& from ) const;

    T getDirectCoefficient( int32_t j, int32_t i ) const;
    T getInverseCoefficient( int32_t j, int32_t i ) const;

    void setDirectCoefficient( int32_t j, int32_t i, 
                               T value,
                               bool update = true );
    void setInverseCoefficient( int32_t j, int32_t i, 
                                T value,
                                bool update = true );

    std::vector< T > getDirectCoefficients() const;

    HomogeneousTransform3d inverse() const;

  protected:

    T _directItems[ 4 ][ 4 ];
    T _inverseItems[ 4 ][ 4 ];

    void invert( T d[ 4 ][ 4 ], T i[ 4 ][ 4 ] );

};


}


template < class T >
dcm::HomogeneousTransform3d< T > operator * ( 
                                   const dcm::HomogeneousTransform3d< T >& t1,
                                   const dcm::HomogeneousTransform3d< T >& t2 );


namespace std
{


ostream& operator << ( ostream& os, 
                       const dcm::HomogeneousTransform3d< float >& t );
ostream& operator << ( ostream& os, 
                       const dcm::HomogeneousTransform3d< double >& t );


}


#endif
