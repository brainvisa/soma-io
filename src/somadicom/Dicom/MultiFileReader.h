#ifndef _MultiFileReader_h_
#define _MultiFileReader_h_


#include <soma-io/Dicom/MultiSliceReader.h>


class DcmDataset;


namespace soma
{


class Directory;


class MultiFileReader : public MultiSliceReader
{

  public:

    MultiFileReader();

    bool read( Directory& directory, 
               Data& data,
               Callback* progress = 0 );

  protected:

    virtual bool readHeader( DcmDataset* dataset );

    bool sortFiles( Directory& directory );

};


}


#endif
