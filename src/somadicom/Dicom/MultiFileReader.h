#ifndef _MultiFileReader_h_
#define _MultiFileReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MultiSliceReader.h>
#else
#include <Dicom/MultiSliceReader.h>
#endif

#include <string>


class DcmDataset;


namespace dcm
{


class MultiFileReader : public MultiSliceReader
{

  public:

    MultiFileReader();

  protected:

    virtual bool readHeader( DcmDataset* dataset );

    bool selectFiles( DirectoryParser& directory,
                      const std::string& seriesInstanceUID,
                      DicomDatasetHeader& datasetHeader );
    virtual bool sortFiles( DicomDatasetHeader& datasetHeader );
    bool readData( DicomDatasetHeader& datasetHeader, DicomProxy& proxy );

};


}


#endif
