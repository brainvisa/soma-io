#ifndef _MultiSliceReader_h_
#define _MultiSliceReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomReader.h>
#else
#include <Dicom/DicomReader.h>
#endif


namespace dcm
{


class HeaderProxy;
class DataInfo;


class MultiSliceReader
{

  public:

    MultiSliceReader();
    virtual ~MultiSliceReader();

    bool getHeader( HeaderProxy& header, 
                    DataInfo& dataInfo,
                    DicomDatasetHeader& datasetHeader );

    void setOrientation( DataInfo& dataInfo );

  protected:

    std::vector< Vector3d< double > > _positions;

};


}


#endif
