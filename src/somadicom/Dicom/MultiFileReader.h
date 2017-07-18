#ifndef _MultiFileReader_h_
#define _MultiFileReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomReader.h>
#include <soma-io/Dicom/MultiSliceReader.h>
#else
#include <Dicom/DicomReader.h>
#include <Dicom/MultiSliceReader.h>
#endif


class DcmDataset;


namespace dcm
{


class MultiFileReader : public DicomReader,
                        public MultiSliceReader
{

  public:

    MultiFileReader();

    virtual bool getHeader( HeaderProxy& header, 
                            DataInfo& dataInfo,
                            DicomDatasetHeader& datasetHeader );

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
