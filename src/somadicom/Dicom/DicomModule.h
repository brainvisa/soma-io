#ifndef _DicomModule_h_
#define _DicomModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#endif


class DcmItem;


namespace soma
{


class DicomDatasetHeader;


class DicomModule
{

  public:

    DicomModule();
    virtual ~DicomModule();

    virtual bool parseItem( DcmItem* dcmItem );
    virtual bool parseHeader( DicomDatasetHeader& datasetHeader );

};


}


#endif

