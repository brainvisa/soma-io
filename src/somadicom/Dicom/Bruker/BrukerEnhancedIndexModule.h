#ifndef _BrukerEnhancedIndexModule_h_
#define _BrukerEnhancedIndexModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/EnhancedIndexModule.h>
#else
#include <Dicom/EnhancedIndexModule.h>
#endif


namespace soma
{


class BrukerEnhancedIndexModule : public EnhancedIndexModule
{

  public:

    BrukerEnhancedIndexModule();

    bool parseItem( DcmItem* dcmItem );

};


}


#endif

