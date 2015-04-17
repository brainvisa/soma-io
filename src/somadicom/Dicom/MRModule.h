#ifndef _MRModule_h_
#define _MRModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#else
#include <Dicom/DicomModule.h>
#endif


namespace dcm
{


class MRModule : public DicomModule
{

  public:

    MRModule();

    bool parseItem( DcmItem* dcmItem );

    double getTR() const;
    double getTE() const;
    double getTI() const;
    double getFlipAngle() const;

  private:

    double _tr;
    double _te;
    double _ti;
    double _flipAngle;

}; 


}


#endif
