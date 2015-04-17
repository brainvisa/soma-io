#ifndef _PhilipsNuclearModule_h_
#define _PhilipsNuclearModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/NuclearModule.h>
#else
#include <Dicom/NuclearModule.h>
#endif


namespace dcm
{


class PhilipsNuclearModule : public NuclearModule
{

  public:

    PhilipsNuclearModule();

    bool parseItem( DcmItem* dcmItem );

};


}


#endif
