#ifndef _DicomReaderFactory_h_
#define _DicomReaderFactory_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Pattern/Singleton.h>
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


class DicomReaderFactory : public Singleton< DicomReaderFactory >
{

  public:

    bool registerReader( DicomReader* reader );

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

    friend class Singleton< DicomReaderFactory >;

    DicomReaderFactory();

    std::map< std::string, std::map< std::string, DicomReader* > > _readers;

  private:

    DicomReader* getReader( const DatasetModule& datasetModule );

};


}


#define RegisterDicomReaderFunction( IMPLEMENTATION )                          \
static bool init_##IMPLEMENTATION =                                            \
                        dcm::DicomReaderFactory::getInstance().registerReader( \
                        &dcm::IMPLEMENTATION::getInstance() )


#endif
