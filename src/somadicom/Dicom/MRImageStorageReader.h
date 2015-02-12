#ifndef _MRImageStorageReader_h_
#define _MRImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiFileReader.h>
#else
#include <Dicom/MultiFileReader.h>
#endif


class DcmDataset;


namespace soma
{


class MRImageStorageReader : public MultiFileReader
{

  public:

    std::string getStorageUID();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& info,
                            DicomDatasetHeader& datasetHeader );

  protected:

    MRImageStorageReader();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomDatasetHeader& datasetHeader,
                           DicomProxy& proxy );

};


}


#endif
