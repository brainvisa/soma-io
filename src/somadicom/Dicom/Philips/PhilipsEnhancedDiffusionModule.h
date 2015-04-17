#ifndef _PhilipsEnhancedDiffusionModule_h_
#define _PhilipsEnhancedDiffusionModule_h_

#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DiffusionModule.h>
#else
#include <Dicom/DiffusionModule.h>
#endif


namespace dcm
{


class PhilipsEnhancedDiffusionModule : public DiffusionModule
{

  public:

    PhilipsEnhancedDiffusionModule();

    bool parseItem( DcmItem* dcmItem );

  protected:

    int32_t getStep( DicomDatasetHeader& datasetHeader );

};


}


#endif
