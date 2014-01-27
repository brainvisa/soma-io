#ifndef _PatientOrientation_h_
#define _PatientOrientation_h_


#include <Transformation/AxialTransformation3d.h>
#include <Transformation/DicomTransformation3d.h>
#include <Utils/StdInt.h>


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
              double pixelSpacingX,
              double pixelSpacingY,
              double pixelSpacingZ );
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

    int32_t m_onDiskSizeX;
    int32_t m_onDiskSizeY;
    int32_t m_onDiskSizeZ;
    double m_onDiskResolutionX;
    double m_onDiskResolutionY;
    double m_onDiskResolutionZ;

    int32_t m_sizeX;
    int32_t m_sizeY;
    int32_t m_sizeZ;
    double m_resolutionX;
    double m_resolutionY;
    double m_resolutionZ;

    Vector m_normalVector;

    AxialTransformation3d m_axialTransformation;
    DicomTransformation3d m_dicomTransformation;

};


}


#endif
