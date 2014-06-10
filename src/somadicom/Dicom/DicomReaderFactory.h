#ifndef _DicomReaderFactory_h_
#define _DicomReaderFactory_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Pattern/Singleton.h>

#include <vector>
#include <string>
#include <map>


namespace soma
{


class DicomReader;
class DirectoryParser;
class DataInfo;
class Data;
class Callback;


class DicomReaderFactory : public Singleton< DicomReaderFactory >
{

  public:

    bool registerReader( DicomReader* reader );

    bool check( std::string manufacturer,
                std::string storageUID,
                DirectoryParser& directory,
                std::vector< std::string >& fileList,
                DataInfo& dataInfo );
    bool read( std::string manufacturer,
               std::string storageUID,
               const std::vector< std::string >& fileList,
               Data& data,
               Callback* progress = 0 );

  protected:

    friend class Singleton< DicomReaderFactory >;

    DicomReaderFactory();

    std::map< std::string, std::map< std::string, DicomReader* > > m_readers;

  private:

    DicomReader* getReader( std::string manufacturer,
                            std::string storageUID );

};


}


#define RegisterDicomReaderFunction( IMPLEMENTATION )                          \
static bool init_##IMPLEMENTATION =                                            \
                      soma::DicomReaderFactory::getInstance().registerReader(  \
                      &soma::IMPLEMENTATION::getInstance() )


#endif
