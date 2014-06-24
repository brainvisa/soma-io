#ifndef _MultiFileReader_h_
#define _MultiFileReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiSliceReader.h>


class DcmDataset;


namespace soma
{


class DirectoryParser;


class MultiFileReader : public MultiSliceReader
{

  public:

    MultiFileReader();

    bool check( DirectoryParser& directory,
                std::vector< std::string >& fileList,
                DataInfo& dataInfo );
    bool read( const std::vector< std::string >& fileList, 
               Data& data,
               Callback* progress = 0 );

  protected:

    virtual bool readHeader( DcmDataset* dataset );

    std::vector< std::string > sortFiles( DirectoryParser& directory );

};


}


#endif
