#ifndef _DicomReader_h_
#define _DicomReader_h_


#include <soma-io/Container/DataInfo.h>

#include <string>
#include <vector>


class DcmDataset;


namespace soma
{


class Directory;
class Data;
class Callback;


class DicomReader
{

  public:

    DicomReader();
    virtual ~DicomReader();  

    virtual std::string getManufacturerName();
    virtual std::string getStorageUID() = 0;

    virtual bool read( Directory& directory, 
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
