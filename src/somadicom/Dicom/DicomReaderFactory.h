#ifndef _DicomReaderFactory_h_
#define _DicomReaderFactory_h_


#include <Pattern/Singleton.h>

#include <string>
#include <map>


namespace soma
{


class DicomReader;
class Directory;
class Data;
class Callback;


class DicomReaderFactory : public Singleton< DicomReaderFactory >
{

  public:

    bool registerReader( DicomReader* reader );

    bool read( std::string manufacturer,
               std::string storageUID,
               Directory& directory,
               Data& data,
               Callback* progress = 0 );

  protected:

    friend class Singleton< DicomReaderFactory >;

    DicomReaderFactory();

    std::map< std::string, std::map< std::string, DicomReader* > > m_readers;

};


}


#define RegisterDicomReaderFunction( IMPLEMENTATION )                          \
static bool init_##IMPLEMENTATION =                                            \
                      soma::DicomReaderFactory::getInstance().registerReader(  \
                      &soma::IMPLEMENTATION::getInstance() )


#endif
