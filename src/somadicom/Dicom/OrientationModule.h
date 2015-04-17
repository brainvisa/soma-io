#ifndef _OrientationModule_h_
#define _OrientationModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Container/Vector3d.h>
#else
#include <Dicom/DicomModule.h>
#include <Container/Vector3d.h>
#endif


class DcmDataset;


namespace dcm
{


class OrientationModule : public DicomModule
{

  public:

    OrientationModule();

    bool parseItem( DcmItem* item );
    virtual bool parseDataset( DcmDataset* dataset );

    Vector3d< double > getRowCosine() const;
    Vector3d< double > getColumnCosine() const;

  private:

    Vector3d< double > _rowCosine;
    Vector3d< double > _columnCosine;

};


}


#endif
