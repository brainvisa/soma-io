#ifndef _PhilipsDiffusionModule_h_
#define _PhilipsDiffusionModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DiffusionModule.h>
#else
#include <Dicom/DiffusionModule.h>
#endif


namespace dcm
{


class PhilipsDiffusionModule : public DiffusionModule
{

  public:

    PhilipsDiffusionModule();

    bool parseItem( DcmItem* dcmItem );

  protected:

    int32_t getStep( DicomDatasetHeader& datasetHeader );

};


}


#endif
