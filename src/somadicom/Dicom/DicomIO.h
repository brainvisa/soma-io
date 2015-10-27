#ifndef _DicomIO_h_
#define _DicomIO_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Pattern/Singleton.h>
#include <soma-io/Dicom/DatasetModule.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Dicom/DicomWriterFactory.h>
#else
#include <Pattern/Singleton.h>
#include <Dicom/DatasetModule.h>
#include <Dicom/DicomReaderFactory.h>
#include <Dicom/DicomWriterFactory.h>
#endif

#include <string>


namespace dcm
{


class DataInfo;
class DicomProxy;
class DicomDatasetHeader;
class HeaderProxy;


class DicomIO : public Singleton< DicomIO >
{

  public:

    bool registerReader( DicomReader* reader );
    bool registerWriter( DicomWriter* writer );

    bool analyze( const std::string& fileName, DataInfo& dataInfo );
    bool check( const std::string& fileName,
                DataInfo& dataInfo,
                DicomDatasetHeader& datasetHeader );
    bool getHeader( HeaderProxy& header, 
                    DataInfo& dataInfo,
                    DicomDatasetHeader& datasetHeader );
    bool read( DicomDatasetHeader& datasetHeader, DicomProxy& proxy );
    bool read( const std::string& fileName, DicomProxy& proxy );

    bool write( const std::string& fileName,
                DicomProxy& proxy,
                bool forceSecondaryCapture = false );

  protected:

    DicomIO();
    virtual ~DicomIO();

    friend class Singleton< DicomIO >;

  private:

    bool checkDicom( const std::string& fileName );

    DatasetModule _datasetModule;
    DicomReaderFactory _readerFactory;
    DicomWriterFactory _writerFactory;

};


}


#define RegisterDicomReaderFunction( IMPLEMENTATION )                          \
static bool init_##IMPLEMENTATION =                                            \
                                   dcm::DicomIO::getInstance().registerReader( \
                                   &dcm::IMPLEMENTATION::getInstance() )


#define RegisterDicomWriterFunction( IMPLEMENTATION )                          \
static bool init_##IMPLEMENTATION =                                            \
                                   dcm::DicomIO::getInstance().registerWriter( \
                                   &dcm::IMPLEMENTATION::getInstance() )


#endif
