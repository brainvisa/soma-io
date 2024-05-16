#ifdef SOMA_IO_DICOM
#include <soma-io/config/somadicom.h>
#include <soma-io/Dicom/DicomIO.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/ModalityLutModule.h>
#include <soma-io/Dicom/EnhancedModalityLutModule.h>
#include <soma-io/System/DirectoryParser.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#ifdef SOMA_DICOM_JPEG2000_SUPPORT
#include <soma-io/Dicom/DcmtkJpeg2000/djdecode.h>
#endif
#else
#include <Dicom/DicomIO.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/ModalityLutModule.h>
#include <Dicom/EnhancedModalityLutModule.h>
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
}


dcm::DicomIO::~DicomIO()
{
}


carto::Mutex & dcm::DicomIO::mutex()
{
  static carto::Mutex m( carto::Mutex::Recursive );
  return m;
}


bool dcm::DicomIO::registerReader( dcm::DicomReader* reader )
{

  mutex().lock();
  bool r = _readerFactory.add( reader );
  mutex().unlock();
  return r;
}


bool dcm::DicomIO::registerWriter( dcm::DicomWriter* writer )
{

  mutex().lock();
  bool r = _writerFactory.add( writer );
  mutex().unlock();
  return r;
}


bool dcm::DicomIO::analyze( const std::string& fileName,
                            dcm::DataInfo& dataInfo,
                            bool applyModalityLut )
{

  dcm::DirectoryParser directory( fileName );
  std::string selectedFile = directory.getSelectedFile();

  if ( checkDicom( selectedFile ) )
  {

    DcmFileFormat header;

    if ( header.loadFile( selectedFile.c_str() ).good() )
    {

      DcmDataset* dataset = header.getDataset();

      if ( _datasetModule.parseItem( dataset ) )
      {

        int32_t bits = _datasetModule.getBitsAllocated();

        dataInfo._depth = bits < 8 ? 8 : bits;
        dataInfo._spp = _datasetModule.getSamplesPerPixel();
        dataInfo._bpp = ( dataInfo._depth >> 3 ) * dataInfo._spp;
        dataInfo._pixelRepresentation = _datasetModule.getPixelRepresentation();

        if ( applyModalityLut )
        {

          dcm::ModalityLutModule modalityLutModule;
          dcm::EnhancedModalityLutModule enhancedModalityLutModule;

          if ( modalityLutModule.parseDataset( dataset ) ||
               enhancedModalityLutModule.parseDataset( dataset ) )
          {

            dataInfo._depth = 32;
            dataInfo._modalityLut = true;

          }

        }

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

    return _readerFactory.check( _datasetModule,
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

  return _readerFactory.getHeader( _datasetModule, 
                                   header,
                                   dataInfo,
                                   datasetHeader );

}


bool dcm::DicomIO::read( dcm::DicomDatasetHeader& datasetHeader, 
                         dcm::DicomProxy& proxy )
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

  bool status = _readerFactory.read( _datasetModule, datasetHeader, proxy );

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

  return status;

}


bool dcm::DicomIO::read( const std::string& fileName,
                         dcm::DicomProxy& proxy,
                         bool applyModalityLut )
{

  bool status = false;

  if ( !fileName.empty() )
  {

    dcm::DataInfo& info = proxy.getDataInfo();

    if ( analyze( fileName, info, applyModalityLut ) )
    {

      dcm::DicomDatasetHeader datasetHeader( info );

      if ( check( fileName, info, datasetHeader ) )
      {

        status = read( datasetHeader, proxy );

      }

    }

  }

  return status;

}


bool dcm::DicomIO::write( const std::string& fileName,
                          dcm::DicomProxy& proxy,
                          bool forceSecondaryCapture )
{

  bool status = false;

  if ( !fileName.empty() )
  {

    status = _writerFactory.write( fileName, 
                                   proxy,
                                   forceSecondaryCapture );

  }

  return status;

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
      uint32_t* dcmTestInt = (uint32_t*)dcmTest;
      uint32_t* valueInt = (uint32_t*)value;

      ifs.seekg( 0, ifs.end );

      length = ifs.tellg();

      // first, test signature
      if ( length > 132 )
      {

        ifs.seekg( 128, ifs.beg );
        ifs.read( (char*)value, 4 );

      }

      if ( *dcmTestInt == *valueInt )
      {

        ifs.close();
        return true;

      }
      else
      {

        uint8_t dcmTest2[] = { 0x08, 0x00, 0x00, 0x00, 0x55, 0x4C, 0x04, 0x00 };
        uint8_t value2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        uint64_t* dcmTestInt2 = (uint64_t*)dcmTest2;
        uint64_t* valueInt2 = (uint64_t*)value2;

        // second, test explicit dataset
        if ( length > 8 )
        {

          ifs.seekg( 0, ifs.beg );
          ifs.read( (char*)value2, 8 );

        }

        if ( *dcmTestInt2 == *valueInt2 )
        {

          ifs.close();
          return true;

        }
        else
        {

          uint8_t dcmTest3[] = { 0x08, 0x00, 0x00, 0x00, 
                                 0x04, 0x00, 0x00, 0x00 };
          uint8_t value3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
          uint64_t* dcmTestInt3 = (uint64_t*)dcmTest3;
          uint64_t* valueInt3 = (uint64_t*)value3;

          // third, test implicit dataset
          if ( length > 8 )
          {

            ifs.seekg( 0, ifs.beg );
            ifs.read( (char*)value3, 8 );

          }

          if ( *dcmTestInt3 == *valueInt3 )
          {

            ifs.close();
            return true;

          }

        }

      }

      ifs.close();

    }

  }

  return false;

}
