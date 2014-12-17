#ifndef _DicomReader_h_
#define _DicomReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Container/DataInfo.h>

#include <string>
#include <vector>


class DcmDataset;


namespace soma
{


class DirectoryParser;
class DicomProxy;
class Callback;
class HeaderProxy;


class DicomReader
{

  public:

    DicomReader();
    virtual ~DicomReader();  

    virtual std::string getManufacturerName();
    virtual std::string getStorageUID() = 0;

    virtual bool check( DirectoryParser& directory,
                        std::vector< std::string >& fileList,
                        DataInfo& dataInfo );
    virtual bool getHeader( HeaderProxy& header, DataInfo& dataInfo );
    virtual bool read( const std::vector< std::string >& fileList, 
                       DicomProxy& proxy,
                       Callback* progress = 0 );

  protected:

    bool readHeader( const std::string& fileName );

    virtual std::vector< std::string > sortFiles( DirectoryParser& directory );
    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomProxy& proxy, Callback* progress ) = 0;
   
    DataInfo* m_dataInfo;
    std::string m_seriesInstanceUID;
    std::vector< std::string > m_slices;

};


}


#endif
