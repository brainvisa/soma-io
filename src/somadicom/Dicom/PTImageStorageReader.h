#ifndef _PTImageStorageReader_h_
#define _PTImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiFileReader.h>
#else
#include <Dicom/MultiFileReader.h>
#endif


class DcmDataset;


namespace soma
{


class PTImageStorageReader : public MultiFileReader
{

  public:

    std::string getStorageUID();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& info,
                            DicomDatasetHeader& datasetHeader );

  protected:

    PTImageStorageReader();

};


}


#endif
