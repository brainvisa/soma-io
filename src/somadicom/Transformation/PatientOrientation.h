#ifndef _PatientOrientation_h_
#define _PatientOrientation_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Container/BoundingBox.h>
#include <soma-io/Transformation/AxialTransformation3d.h>
#include <soma-io/Transformation/DicomTransformation3d.h>
#else
#include <Container/BoundingBox.h>
#include <Transformation/AxialTransformation3d.h>
#include <Transformation/DicomTransformation3d.h>
#endif


namespace soma
{


class PatientOrientation
{

  public:

    PatientOrientation();
    PatientOrientation( const PatientOrientation& other );
    virtual ~PatientOrientation();

    void set( const Vector& rowVector,
              const Vector& columnVector,
              const Vector& normalVector,
              const Vector& origin,
              int32_t sizeX,
              int32_t sizeY,
              int32_t sizeZ,
              const Vector& resolution );
    void setOrigin( const Vector& origin );

    void getOnDiskSize( int32_t& sizeX, int32_t& sizeY, int32_t& sizeZ );
    void getSize( int32_t& sizeX, int32_t& sizeY, int32_t& sizeZ );
    void getResolution( double& resolutionX,
                        double& resolutionY,
                        double& resolutionZ );

    BoundingBox< int32_t > getDirectBoundingBox( 
                              const BoundingBox< int32_t >& boundingBox ) const;
    BoundingBox< int32_t > getInverseBoundingBox( 
                              const BoundingBox< int32_t >& boundingBox ) const;

    void getDirect( const int32_t fromX, 
                    const int32_t fromY, 
                    const int32_t fromZ,
                    int32_t& toX, 
                    int32_t& toY, 
                    int32_t& toZ ) const;
    void getInverse( const int32_t toX, 
                     const int32_t toY, 
                     const int32_t toZ, 
                     int32_t& fromX, 
                     int32_t& fromY, 
                     int32_t& fromZ ) const;

    const Transformation3d& getAxialTransformation() const;

    Transformation3d getReferential() const;

  private:

    bool _axialAcquisition;

    Vector _normalVector;

    int32_t _onDiskSizeX;
    int32_t _onDiskSizeY;
    int32_t _onDiskSizeZ;
    Vector _onDiskResolution;

    int32_t _sizeX;
    int32_t _sizeY;
    int32_t _sizeZ;
    Vector _resolution;

    AxialTransformation3d _axialTransformation;
    AxialTransformation3d _flipTransformation;
    DicomTransformation3d _dicomTransformation;

};


}


#endif
