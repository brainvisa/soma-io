#ifndef _USReader_h_
#define _USReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomReader.h>
#else
#include <Dicom/DicomReader.h>
#endif


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
