#ifndef _EnhancedReader_h_
#define _EnhancedReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/SingleFileReader.h>
#include <soma-io/Dicom/MultiSliceReader.h>
#else
#include <Dicom/SingleFileReader.h>
#include <Dicom/MultiSliceReader.h>
#endif

#include <string>


namespace dcm
{


class EnhancedReader : public SingleFileReader,
                       public MultiSliceReader
{

  public:

    EnhancedReader();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& info,
                            DicomDatasetHeader& datasetHeader );

  protected:

    virtual bool readHeader( DcmDataset* dataset );

    virtual bool buildIndexLut( DcmDataset* dataset );

};


}


#endif
