#ifndef _PhilipsEnhancedIndexModule_h_
#define _PhilipsEnhancedIndexModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/EnhancedIndexModule.h>
#else
#include <Dicom/EnhancedIndexModule.h>
#endif


namespace dcm
{


class PhilipsEnhancedIndexModule : public EnhancedIndexModule
{

  public:

    PhilipsEnhancedIndexModule();

    bool parseItem( DcmItem* dcmItem );

};


}


#endif
