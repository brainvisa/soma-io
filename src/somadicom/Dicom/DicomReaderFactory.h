#ifndef _DicomReaderFactory_h_
#define _DicomReaderFactory_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#endif

#include <string>
#include <map>


namespace dcm
{


class DicomReader;
class DatasetModule;
class DirectoryParser;
class DataInfo;
class DicomProxy;
class DicomDatasetHeader;
class HeaderProxy;


class DicomReaderFactory
{

  public:

    DicomReaderFactory();
    virtual ~DicomReaderFactory();

    bool add( DicomReader* reader );

    bool check( const DatasetModule& datasetModule,
                DirectoryParser& directory,
                DataInfo& dataInfo,
                DicomDatasetHeader& datasetHeader );
    bool getHeader( const DatasetModule& datasetModule,
                    HeaderProxy& header,
                    DataInfo& dataInfo,
                    DicomDatasetHeader& datasetHeader );
    bool read( const DatasetModule& datasetModule,
               DicomDatasetHeader& datasetHeader,
               DicomProxy& proxy );

  protected:

    std::map< std::string, std::map< std::string, DicomReader* > > _readers;

  private:

    DicomReader* getReader( const DatasetModule& datasetModule );

};


}


#endif
