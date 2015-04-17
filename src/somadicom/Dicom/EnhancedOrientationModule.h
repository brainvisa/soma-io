#ifndef _EnhancedOrientationModule_h_
#define _EnhancedOrientationModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/OrientationModule.h>
#else
#include <Dicom/OrientationModule.h>
#endif


namespace dcm
{


class EnhancedOrientationModule : public OrientationModule
{

  public:

    EnhancedOrientationModule();

    bool parseDataset( DcmDataset* dataset );

};


}


#endif
