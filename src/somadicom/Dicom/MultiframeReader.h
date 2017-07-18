#ifndef _MultiframeReader_h_
#define _MultiframeReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/SingleFileReader.h>
#else
#include <Dicom/SingleFileReader.h>
#endif


namespace dcm
{


class MultiframeReader : public SingleFileReader
{

  public:

    MultiframeReader();

  protected:


    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomDatasetHeader& datasetHeader,
                           DicomProxy& proxy );
    virtual DicomImage* getDicomImage( DicomProxy& proxy );

};


}


#endif
