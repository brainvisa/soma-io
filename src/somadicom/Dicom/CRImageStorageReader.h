#ifndef _CRImageStorageReader_h_
#define _CRImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiFileReader.h>
#else
#include <Dicom/MultiFileReader.h>
#endif


namespace soma
{


class CRImageStorageReader : public MultiFileReader
{

  public:

    std::string getStorageUID();

  protected:

    CRImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomDatasetHeader& datasetHeader,
                           DicomProxy& proxy );

};


}


#endif
