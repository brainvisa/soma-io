#ifndef _PositionModule_h_
#define _PositionModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Container/Vector3d.h>
#else
#include <Dicom/DicomModule.h>
#include <Container/Vector3d.h>
#endif


namespace dcm
{


class PositionModule : public DicomModule
{

  public:

    PositionModule();

    bool parseItem( DcmItem* item );

    Vector3d< double > getImagePosition() const;

  private:

    Vector3d< double > _imagePosition;

};


}


#endif
