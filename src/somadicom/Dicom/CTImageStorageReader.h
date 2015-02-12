#ifndef _CTImageStorageReader_h_
#define _CTImageStorageReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiFileReader.h>
#else
#include <Dicom/MultiFileReader.h>
#endif


namespace soma
{


class CTImageStorageReader : public MultiFileReader
{

  public:

    std::string getStorageUID();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& info,
                            DicomDatasetHeader& datasetHeader );

  protected:

    CTImageStorageReader();

    virtual bool readData( DicomDatasetHeader& datasetHeader, 
                           DicomProxy& proxy );

};


}


#endif
