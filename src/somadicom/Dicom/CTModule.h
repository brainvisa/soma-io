#ifndef _CTModule_h_
#define _CTModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#else
#include <Dicom/DicomModule.h>
#endif


namespace soma
{


class CTModule : public DicomModule
{

  public:

    CTModule();

    bool parseDataset( DcmDataset* dataset );

    double getReconstructionDiameter() const;

  private:

    double _reconstructionDiameter;

}; 


}


#endif

