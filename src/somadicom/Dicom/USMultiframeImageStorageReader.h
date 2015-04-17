#ifndef _USMultiframeImageStorageReader_h_
#define _USMultiframeImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/USReader.h>
#else
#include <Dicom/USReader.h>
#endif


class DcmDataset;


namespace dcm
{


class DicomDatasetHeader;
class DicomProxy;


class USMultiframeImageStorageReader : public USReader
{

  public:

    std::string getStorageUID();

  protected:

    USMultiframeImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomDatasetHeader& datasetHeader,
                           DicomProxy& proxy );

};


}


#endif
