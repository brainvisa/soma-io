#ifndef _USRegionSpatialModule_h_
#define _USRegionSpatialModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#else
#include <Dicom/DicomModule.h>
#endif


namespace dcm
{


class USRegionSpatialModule : public DicomModule
{

  public:

    USRegionSpatialModule();

    bool parseItem( DcmItem* dcmItem );

    double getPhysicalDeltaX() const;
    double getPhysicalDeltaY() const;

  private:

    double _physicalDeltaX;
    double _physicalDeltaY;

};


}


#endif
