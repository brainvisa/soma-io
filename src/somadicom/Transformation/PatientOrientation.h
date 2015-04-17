#ifndef _PatientOrientation_h_
#define _PatientOrientation_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Container/BoundingBox.h>
#include <soma-io/Transformation/FlipTransform3d.h>
#include <soma-io/Transformation/AxialTransform3d.h>
#include <soma-io/Transformation/DicomTransform3d.h>
#else
#include <Container/BoundingBox.h>
#include <Transformation/FlipTransform3d.h>
#include <Transformation/AxialTransform3d.h>
#include <Transformation/DicomTransform3d.h>
#endif


namespace dcm
{


class PatientOrientation
{

  public:

    PatientOrientation();
    PatientOrientation( const PatientOrientation& other );
    virtual ~PatientOrientation();

    void set( const Vector3d< double >& rowVector,
              const Vector3d< double >& columnVector,
              const Vector3d< double >& origin,
              int32_t sizeX,
              int32_t sizeY,
              int32_t sizeZ,
              const Vector3d< double >& resolution );
    void setOrigin( const Vector3d< double >& origin );

    Vector3d< int32_t > getOnDiskSize() const;
    Vector3d< int32_t > getSize() const;
    Vector3d< double > getResolution() const;

    BoundingBox< int32_t > getDirectBoundingBox( 
                              const BoundingBox< int32_t >& boundingBox ) const;
    BoundingBox< int32_t > getInverseBoundingBox( 
                              const BoundingBox< int32_t >& boundingBox ) const;

    Vector3d< int32_t > getDirect( const int32_t x, 
                                   const int32_t y, 
                                   const int32_t z ) const;
    Vector3d< int32_t > getInverse( const int32_t x, 
                                    const int32_t y, 
                                    const int32_t z ) const;

    const HomogeneousTransform3d< double >& getAxialTransformation() const;

    HomogeneousTransform3d< double > getReferential() const;

  private:

    Vector3d< int32_t > _onDiskSize;
    Vector3d< double > _onDiskResolution;
    Vector3d< int32_t > _size;
    Vector3d< double > _resolution;

    FlipTransform3d< double > _flipTransform;
    AxialTransform3d< double > _axialTransform;
    AxialTransform3d< double > _scaledTransform;
    DicomTransform3d< double > _dicomTransform;

};


}


#endif
