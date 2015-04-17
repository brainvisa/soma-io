#ifndef _USImageStorageReader_h_
#define _USImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/USReader.h>
#else
#include <Dicom/USReader.h>
#endif

#include <string>


class DcmDataset;


namespace dcm
{


class DicomDatasetHeader;
class DicomProxy;


class USImageStorageReader : public USReader
{

  public:

    std::string getStorageUID();

  protected:

    USImageStorageReader();

    virtual bool readData( DicomDatasetHeader& datasetHeader,
                           DicomProxy& proxy );

};


}


#endif
