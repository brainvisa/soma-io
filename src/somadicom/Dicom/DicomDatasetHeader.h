#ifndef _DicomDatasetHeader_h_
#define _DicomDatasetHeader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Utils/StdInt.h>

#include <map>
#include <string>


class DcmDataset;
class DcmFileFormat;


namespace soma
{


class DicomProxy;
class BinaryHeader;


class DicomDatasetHeader
{

  public:

    DicomDatasetHeader( DicomProxy& proxy );
    virtual ~DicomDatasetHeader();

    void add( DcmDataset* dataset, int32_t i );
    void get( DcmDataset& dataset, int32_t i );

    bool getDictionary( std::map< std::string, std::string >& dictionary );
    bool setDictionary( std::map< std::string, std::string >& dictionary );

  private:

    BinaryHeader& m_binaryHeader;

};


}


#endif
