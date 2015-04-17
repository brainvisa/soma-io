#ifndef _DicomDatasetHeader_h_
#define _DicomDatasetHeader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Utils/StdInt.h>
#endif

#include <map>
#include <string>
#include <vector>


class DcmDataset;


namespace dcm
{


class DicomProxy;
class DataInfo;
class BinaryHeader;


class DicomDatasetHeader
{

  public:

    DicomDatasetHeader( DataInfo& dataInfo );
    DicomDatasetHeader( DicomProxy& proxy );
    virtual ~DicomDatasetHeader();

    void clear();
    void allocate( int32_t size );
    int32_t size();

    bool add( DcmDataset* dataset, const std::string& fileName );
    bool add( int32_t i, DcmDataset* dataset, const std::string& fileName );
    void get( DcmDataset& dataset, int32_t i = 0 );
    void trim();

    bool getDictionary( std::map< std::string, std::string >& dictionary );
    bool setDictionary( std::map< std::string, std::string >& dictionary );

    std::vector< int32_t >& getLut();
    std::vector< std::string >& getFileList();

  private:

    BinaryHeader& _binaryHeader;
    std::vector< std::string > _fileList;

};


}


#endif
