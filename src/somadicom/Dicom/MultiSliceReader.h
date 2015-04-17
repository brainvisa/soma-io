#ifndef _MultiSliceReader_h_
#define _MultiSliceReader_h_


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


class MultiSliceReader : public DicomReader
{

  public:

    MultiSliceReader();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& dataInfo,
                            DicomDatasetHeader& datasetHeader );

    void setOrientation();

  protected:

    virtual bool readHeader( DcmDataset* dataset );
    virtual bool readData( DicomDatasetHeader& datasetHeader,
                           DicomProxy& proxy );

    virtual bool buildIndexLut( DcmDataset* dataset );

    std::vector< int32_t > _indexLut;
    std::vector< Vector3d< double > > _positions;

};


}


#endif
