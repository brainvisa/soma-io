#ifndef _USReader_h_
#define _USReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomReader.h>


class DcmDataset;


namespace soma
{


class USReader : public DicomReader
{

  public:

    USReader();

  protected:

    bool readHeader( DcmDataset* dataset );

};


}


#endif
