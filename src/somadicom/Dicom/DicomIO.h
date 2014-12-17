#ifndef _DicomIO_h_
#define _DicomIO_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Pattern/Singleton.h>

#include <vector>
#include <string>


class DcmDataset;


namespace soma
{


class DataInfo;
class DicomProxy;
class DicomDatasetHeader;
class Callback;
class HeaderProxy;


class DicomIO : public Singleton< DicomIO >
{

  public:

    bool analyze( const std::string& fileName, DataInfo& dataInfo );
    bool check( const std::string& fileName,
                std::vector< std::string >& fileList,
                DataInfo& dataInfo,
                DicomDatasetHeader& datasetHeader );
    bool getHeader( HeaderProxy& header, 
                    DataInfo& dataInfo,
                    DicomDatasetHeader& datasetHeader );
    bool read( const std::vector< std::string >& fileList, 
               DicomProxy& proxy, 
               Callback* progress = 0 );

  protected:

    DicomIO();
    virtual ~DicomIO();

    friend class Singleton< DicomIO >;

  private:

    bool getInfo( const std::string& filename,
                  std::string& manufacturer,
                  std::string& sopClassUid );
    bool getInfo( DcmDataset* dataset,
                  std::string& manufacturer,
                  std::string& sopClassUid );

};


}


#endif
