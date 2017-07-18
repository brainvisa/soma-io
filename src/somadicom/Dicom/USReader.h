#ifndef _USReader_h_
#define _USReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#endif


class DcmDataset;


namespace dcm
{


class DataInfo;


class USReader
{

  public:

    USReader();
    virtual ~USReader();

  protected:

    bool readHeader( DcmDataset* dataset, DataInfo& dataInfo );

};


}


#endif
