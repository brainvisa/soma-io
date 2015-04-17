#ifdef SOMA_IO_DICOM
#include <soma-io/config/somadicom.h>
#include <soma-io/Dicom/DicomIO.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/System/DirectoryParser.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#ifdef SOMA_DICOM_JPEG2000_SUPPORT
#include <soma-io/Dicom/DcmtkJpeg2000/djdecode.h>
#endif
#else
#include <Dicom/DicomIO.h>
#include <Dicom/DicomReaderFactory.h>
#include <Dicom/DicomDatasetHeader.h>
#include <System/DirectoryParser.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#include <Dicom/DcmtkJpeg2000/djdecode.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#ifndef SOMA_IO_DICOM
#include <dcmtk/dcmjpls/djdecode.h>
#endif

#include <fstream>


dcm::DicomIO::DicomIO()
            : dcm::Singleton< dcm::DicomIO >()
{

  DcmRLEDecoderRegistration::registerCodecs();
  DJDecoderRegistration::registerCodecs();

#ifdef SOMA_IO_DICOM
#ifdef SOMA_DICOM_JPEG2000_SUPPORT
  DJ2KDecoderRegistration::registerCodecs();
#endif
#else
  DJLSDecoderRegistration::registerCodecs();
  DJ2KDecoderRegistration::registerCodecs();
#endif

}


dcm::DicomIO::~DicomIO()
{

#ifdef SOMA_IO_DICOM
#ifdef SOMA_DICOM_JPEG2000_SUPPORT
  DJ2KDecoderRegistration::cleanup();
#endif
#else
  DJ2KDecoderRegistration::cleanup();
  DJLSDecoderRegistration::cleanup();
#endif
  DJDecoderRegistration::cleanup();
  DcmRLEDecoderRegistration::cleanup();

}


bool dcm::DicomIO::analyze( const std::string& fileName,
                            dcm::DataInfo& dataInfo )
{

  dcm::DirectoryParser directory( fileName );
  std::string selectedFile = directory.getSelectedFile();

  if ( checkDicom( selectedFile ) )
  {

    DcmFileFormat header;

    if ( header.loadFile( selectedFile.c_str() ).good() )
    {

      if ( _datasetModule.parseItem( header.getDataset() ) )
      {

        int32_t bits = _datasetModule.getBitsAllocated();

        dataInfo._depth = bits < 8 ? 8 : bits;
        dataInfo._spp = _datasetModule.getSamplesPerPixel();
        dataInfo._bpp = ( dataInfo._depth >> 3 ) * dataInfo._spp;

        return true;

      }

    }

  }

  return false;

}


bool dcm::DicomIO::check( const std::string& fileName,
                          dcm::DataInfo& dataInfo,
                          dcm::DicomDatasetHeader& datasetHeader )

{

  dcm::DirectoryParser directory( fileName );
  std::string selectedFile = directory.getSelectedFile();

  if ( !selectedFile.empty() )
  {

    return dcm::DicomReaderFactory::getInstance().check( _datasetModule,
                                                         directory, 
                                                         dataInfo,
                                                         datasetHeader );

  }

  return false;

}


bool dcm::DicomIO::getHeader( dcm::HeaderProxy& header, 
                              dcm::DataInfo& dataInfo,
                              dcm::DicomDatasetHeader& datasetHeader )
{

  return dcm::DicomReaderFactory::getInstance().getHeader( _datasetModule, 
                                                           header,
                                                           dataInfo,
                                                           datasetHeader );

}


bool dcm::DicomIO::read( dcm::DicomDatasetHeader& datasetHeader, 
                         dcm::DicomProxy& proxy )
{

  return dcm::DicomReaderFactory::getInstance().read( _datasetModule,
                                                      datasetHeader, 
                                                      proxy );

}


bool dcm::DicomIO::checkDicom( const std::string& fileName )
{

  if ( !fileName.empty() )
  {

    std::ifstream ifs( fileName.c_str(), std::ifstream::binary );

    if ( ifs.good() )
    {

      int32_t length = 0;
      uint8_t dcmTest[] = { 0x44, 0x49, 0x43, 0x4D };
      uint8_t value[] = { 0, 0, 0, 0 };
      int32_t* dcmTestInt = (int32_t*)dcmTest;
      int32_t* valueInt = (int32_t*)value;

      ifs.seekg( 0, ifs.end );

      length = ifs.tellg();

      if ( length > 132 )
      {

        ifs.seekg( 128, ifs.beg );
        ifs.read( (char*)value, 4 );

      }

      ifs.close();

      if ( *dcmTestInt == *valueInt )
      {

        return true;

      }

    }

  }

  return false;

}
