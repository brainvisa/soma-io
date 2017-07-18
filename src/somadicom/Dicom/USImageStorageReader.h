#ifndef _USImageStorageReader_h_
#define _USImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/SingleFileReader.h>
#include <soma-io/Dicom/USReader.h>
#else
#include <Dicom/SingleFileReader.h>
#include <Dicom/USReader.h>
#endif


namespace dcm
{


class USImageStorageReader : public SingleFileReader,
                             public USReader
{

  public:

    std::string getStorageUID();

  protected:

    USImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );

};


}


#endif
