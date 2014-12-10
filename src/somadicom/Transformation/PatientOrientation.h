#ifndef _PatientOrientation_h_
#define _PatientOrientation_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Transformation/AxialTransformation3d.h>
#include <soma-io/Transformation/DicomTransformation3d.h>
#include <soma-io/Utils/StdInt.h>


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

    void getDirect( const int32_t fromX, 
                    const int32_t fromY, 
                    const int32_t fromZ,
                    int32_t& toX, 
                    int32_t& toY, 
                    int32_t& toZ );
    void getInverse( const int32_t toX, 
                     const int32_t toY, 
                     const int32_t toZ, 
                     int32_t& fromX, 
                     int32_t& fromY, 
                     int32_t& fromZ );
		     
    Transformation3d getReferential() const;

  private:

    bool m_axialAcquisition;

    Vector m_normalVector;

    int32_t m_onDiskSizeX;
    int32_t m_onDiskSizeY;
    int32_t m_onDiskSizeZ;
    Vector m_onDiskResolution;

    int32_t m_sizeX;
    int32_t m_sizeY;
    int32_t m_sizeZ;
    Vector m_resolution;

    AxialTransformation3d m_axialTransformation;
    DicomTransformation3d m_dicomTransformation;

};


}


#endif
