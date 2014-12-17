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
class DicomProxy;
class Callback;
class HeaderProxy;


class DicomReaderFactory : public Singleton< DicomReaderFactory >
{

  public:

    bool registerReader( DicomReader* reader );

    bool check( const std::string& manufacturer,
                const std::string& storageUID,
                DirectoryParser& directory,
                std::vector< std::string >& fileList,
                DataInfo& dataInfo );
    bool getHeader( const std::string& manufacturer,
                    const std::string& storageUID,
                    HeaderProxy& header,
                    DataInfo& dataInfo );
    bool read( const std::string& manufacturer,
               const std::string& storageUID,
               const std::vector< std::string >& fileList,
               DicomProxy& proxy,
               Callback* progress = 0 );

  protected:

    friend class Singleton< DicomReaderFactory >;

    DicomReaderFactory();

    std::map< std::string, std::map< std::string, DicomReader* > > m_readers;

  private:

    DicomReader* getReader( const std::string& manufacturer,
                            const std::string& storageUID );

};


}


#define RegisterDicomReaderFunction( IMPLEMENTATION )                          \
static bool init_##IMPLEMENTATION =                                            \
                      soma::DicomReaderFactory::getInstance().registerReader(  \
                      &soma::IMPLEMENTATION::getInstance() )


#endif
