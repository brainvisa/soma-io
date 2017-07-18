#ifndef _SingleFileReader_h_
#define _SingleFileReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomReader.h>
#else
#include <Dicom/DicomReader.h>
#endif


class DcmDataset;


namespace dcm
{


class HeaderProxy;
class DicomImage;


class SingleFileReader : public DicomReader
{

  public:

    SingleFileReader();

  protected:

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomDatasetHeader& datasetHeader,
                           DicomProxy& proxy );

    virtual bool buildIndexLut( DcmDataset* dataset );
    virtual DicomImage* getDicomImage( DicomProxy& proxy );

    std::vector< int32_t > _indexLut;
    std::vector< double > _slope;
    std::vector< double > _intercept;

};


}


#endif
