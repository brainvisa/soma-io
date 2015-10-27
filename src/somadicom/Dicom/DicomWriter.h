#ifndef _DicomWriter_h_
#define _DicomWriter_h_


#include <string>


namespace dcm
{


class DicomProxy;


class DicomWriter
{

  public:

    DicomWriter();
    virtual ~DicomWriter();

    virtual std::string getStorageUID() = 0;

    virtual bool write( const std::string& fileName, DicomProxy& proxy );

  protected:

    virtual bool writeDatasets( const std::string& directoryName,
                                const std::string& fileName,
                                DicomProxy& proxy ) = 0;

};


}


#endif
