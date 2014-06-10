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
class Data;
class Callback;


class DicomReader
{

  public:

    DicomReader();
    virtual ~DicomReader();  

    virtual std::string getManufacturerName();
    virtual std::string getStorageUID() = 0;

    virtual std::vector< std::string > check( DirectoryParser& directory,
                                              DataInfo& dataInfo );
    virtual bool read( const std::vector< std::string >& fileList, 
                       Data& data,
                       Callback* progress = 0 );

  protected:

    bool readHeader( const std::string& fileName );

    void initialize();

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( Data& data, Callback* progress ) = 0;
   
    DataInfo m_dataInfo;
    std::string m_seriesInstanceUID;

    std::vector< std::string > m_slices;

};


}


#endif
