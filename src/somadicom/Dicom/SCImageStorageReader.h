#ifndef _SCImageStorageReader_h_
#define _SCImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiFileReader.h>
#else
#include <Dicom/MultiFileReader.h>
#endif


namespace soma
{


class SCImageStorageReader : public MultiFileReader
{

  public:

    std::string getStorageUID();

  protected:

    SCImageStorageReader();

    virtual bool readData( DicomDatasetHeader& datasetHeader,
                           DicomProxy& proxy );

};


}


#endif
