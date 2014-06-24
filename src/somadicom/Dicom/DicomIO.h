#ifndef _DicomIO_h_
#define _DicomIO_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Pattern/Singleton.h>

#include <vector>
#include <string>


namespace soma
{


class DataInfo;
class Data;
class Callback;


class DicomIO : public Singleton< DicomIO >
{

  public:

    bool check( const std::string& fileName,
                std::vector< std::string >& fileList,
                DataInfo& dataInfo );
    bool read( const std::vector< std::string >& fileList, 
               Data& data, 
               Callback* progress = 0 );

  protected:

    DicomIO();
    virtual ~DicomIO();

    friend class Singleton< DicomIO >;

  private:

    bool getInfo( const std::string& filename,
                  std::string& manufacturer,
                  std::string& sopClassUid );

};


}


#endif
