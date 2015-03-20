#ifndef _SiemensDiffusionModule_h_
#define _SiemensDiffusionModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DiffusionModule.h>
#else
#include <Dicom/DiffusionModule.h>
#endif


namespace soma
{


class SiemensDiffusionModule : public DiffusionModule
{

  public:

    SiemensDiffusionModule();

    bool parseItem( DcmItem* dcmItem );

    int32_t getStep( DicomDatasetHeader& datasetHeader );

  protected:

};


}


#endif

