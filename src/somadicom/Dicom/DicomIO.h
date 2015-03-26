#ifndef _DicomIO_h_
#define _DicomIO_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Pattern/Singleton.h>
#include <soma-io/Dicom/DatasetModule.h>
#else
#include <Pattern/Singleton.h>
#include <Dicom/DatasetModule.h>
#endif

#include <string>


namespace soma
{


class DataInfo;
class DicomProxy;
class DicomDatasetHeader;
class HeaderProxy;


class DicomIO : public Singleton< DicomIO >
{

  public:

    bool analyze( const std::string& fileName, DataInfo& dataInfo );
    bool check( const std::string& fileName,
                DataInfo& dataInfo,
                DicomDatasetHeader& datasetHeader );
    bool getHeader( HeaderProxy& header, 
                    DataInfo& dataInfo,
                    DicomDatasetHeader& datasetHeader );
    bool read( DicomDatasetHeader& datasetHeader, DicomProxy& proxy );

  protected:

    DicomIO();
    virtual ~DicomIO();

    friend class Singleton< DicomIO >;

  private:

    bool checkDicom( const std::string& fileName );

    DatasetModule _datasetModule;

};


}


#endif
