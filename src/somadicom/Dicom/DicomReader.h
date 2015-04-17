#ifndef _DicomReader_h_
#define _DicomReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Container/DataInfo.h>
#else
#include <Container/DataInfo.h>
#endif

#include <string>
#include <vector>


class DcmDataset;


namespace dcm
{


class DatasetModule;
class DirectoryParser;
class DicomProxy;
class DicomDatasetHeader;
class HeaderProxy;


class DicomReader
{

  public:

    DicomReader();
    virtual ~DicomReader();  

    virtual std::string getManufacturerName();
    virtual std::string getStorageUID() = 0;

    virtual bool check( const DatasetModule& datasetModule,
                        DirectoryParser& directory,
                        DataInfo& dataInfo,
                        DicomDatasetHeader& datasetHeader );
    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& dataInfo,
                            DicomDatasetHeader& datasetHeader );
    virtual bool read( DicomDatasetHeader& datasetHeader, DicomProxy& proxy );

  protected:

    bool readHeader( DicomDatasetHeader& datasetHeader );

    virtual void setOrientation();
    virtual bool selectFiles( DirectoryParser& directory,
                              const std::string& seriesIntanceUID,
                              DicomDatasetHeader& datasetHeader ); 
    virtual bool sortFiles( DicomDatasetHeader& datasetHeader );
    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomDatasetHeader& datasetHeader,
                           DicomProxy& proxy ) = 0;
   
    DataInfo* _dataInfo;

};


}


#endif
