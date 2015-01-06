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

  protected:

    virtual bool readHeader( DcmDataset* dataset );

    virtual std::vector< std::string > sortFiles( DirectoryParser& directory );

};


}


#endif
