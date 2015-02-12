#ifndef _MultiSliceReader_h_
#define _MultiSliceReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomReader.h>
#else
#include <Dicom/DicomReader.h>
#endif


namespace soma
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

    std::vector< Vector > _positions;
    Vector _origin;

};


}


#endif
