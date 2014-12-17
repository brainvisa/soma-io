#ifndef _PTImageStorageReader_h_
#define _PTImageStorageReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiFileReader.h>

#include <map>


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

    virtual bool readData( DicomProxy& proxy, Callback* progress = 0 );

    std::map< std::string, std::string > m_unitNames;

};


}


#endif
