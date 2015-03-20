#ifndef _GEDiffusionModule_h_
#define _GEDiffusionModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DiffusionModule.h>
#else
#include <Dicom/DiffusionModule.h>
#endif


namespace soma
{


class GEDiffusionModule : public DiffusionModule
{

  public:

    GEDiffusionModule();

    bool parseItem( DcmItem* dcmItem );

};


}


#endif

