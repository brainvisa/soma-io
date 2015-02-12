#ifndef _EnhancedModalityLutModule_h_
#define _EnhancedModalityLutModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/ModalityLutModule.h>
#else
#include <Dicom/ModalityLutModule.h>
#endif

#include <vector>


namespace soma
{


class EnhancedModalityLutModule : public ModalityLutModule
{

  public:

    EnhancedModalityLutModule();

    bool parseDataset( DcmDataset* dataset );

};


}


#endif

