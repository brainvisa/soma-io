#ifndef _AcquisitionModule_h_
#define _AcquisitionModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#else
#include <Dicom/DicomModule.h>
#endif


namespace soma
{


class AcquisitionModule : public DicomModule
{

  public:

    AcquisitionModule();

    bool parseItem( DcmItem* dcmItem );

    double getSliceThickness() const;
    double getSpacingBetweenSlices() const;

  private:

    double _sliceThickness;
    double _spacingBetweenSlices;

};


}


#endif

