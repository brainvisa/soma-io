#ifndef _AxialTransform_h_
#define _AxialTransform_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Transformation/HomogeneousTransform3d.h>
#else
#include <Transformation/HomogeneousTransform3d.h>
#endif


namespace dcm
{


template < class T >
class AxialTransform3d : public HomogeneousTransform3d< T >
{

  public:

    typedef void (AxialTransform3d< T >::*TPointer)( const Vector3d< T >& in,
                                                     Vector3d< T >& out ) const;

    AxialTransform3d();
    AxialTransform3d( const Vector3d< T >& rowCosine,
                      const Vector3d< T >& columnCosine,
                      const Vector3d< T >& origin = Vector3d< T >( 0, 0, 0 ) );
    AxialTransform3d( const AxialTransform3d< T >& other );

    void setTranslation( const Vector3d< T >& translation );

    void getDirect( const Vector3d< T >& from, Vector3d< T >& to ) const;
    void getInverse( const Vector3d< T >& to, Vector3d< T >& from ) const;

  protected:

    void initialize( const Vector3d< T >& normalVector );
    void getDirectAxial( const Vector3d< T >& in, Vector3d< T >& out ) const;
    void getDirectSagittal( const Vector3d< T >& in, Vector3d< T >& out ) const;
    void getDirectCoronal( const Vector3d< T >& in, Vector3d< T >& out ) const;
    void getInverseAxial( const Vector3d< T >& in, Vector3d< T >& out ) const;
    void getInverseSagittal( const Vector3d< T >& in, 
                             Vector3d< T >& out ) const;
    void getInverseCoronal( const Vector3d< T >& in, Vector3d< T >& out ) const;

    TPointer _directPointer;
    TPointer _inversePointer;

};


}


#endif
