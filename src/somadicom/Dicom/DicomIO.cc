#include <soma-io/config/somadicom.h>
#include <soma-io/Dicom/DicomIO.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/System/DirectoryParser.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Pattern/Callback.h>


#ifdef SOMA_DICOM_JPEG2000_SUPPORT
  #include <soma-io/Dicom/DcmtkJpeg2000/djdecode.h>
#endif

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#ifdef SOMA_DICOM_JPEG2000_SUPPORT
#include <dcmtk/dcmjpls/djdecode.h>
#endif
#include <dcmtk/dcmimage/diregist.h>


soma::DicomIO::DicomIO()
             : soma::Singleton< soma::DicomIO >()
{

  DcmRLEDecoderRegistration::registerCodecs();
  DJDecoderRegistration::registerCodecs();
  //DJLSDecoderRegistration::registerCodecs();
  
#ifdef SOMA_DICOM_JPEG2000_SUPPORT
  DJ2KDecoderRegistration::registerCodecs();
#endif

}


soma::DicomIO::~DicomIO()
{

#ifdef SOMA_DICOM_JPEG2000_SUPPORT
  DJ2KDecoderRegistration::cleanup();
#endif
  DJDecoderRegistration::cleanup();
  //DJLSDecoderRegistration::cleanup();
  DcmRLEDecoderRegistration::cleanup();

}


bool soma::DicomIO::check( const std::string& fileName,
                           std::vector< std::string >& fileList,
                           soma::DataInfo& dataInfo )
{

  soma::DirectoryParser directory( fileName );
  std::string selectedFile = directory.getSelectedFile();

  if ( !selectedFile.empty() )
  {

    std::string manufacturer;
    std::string sopClassUid;

    dataInfo.clear();

    if ( getInfo( selectedFile, manufacturer, sopClassUid ) )
    {

      return soma::DicomReaderFactory::getInstance().check( manufacturer,
                                                            sopClassUid, 
                                                            directory, 
                                                            fileList,
                                                            dataInfo );

    }

  }

  return false;

}


bool soma::DicomIO::read( const std::vector< std::string >& fileList, 
                          soma::DicomProxy& proxy,
                          soma::Callback* progress )
{

  if ( !fileList.empty() )
  {

    std::string manufacturer;
    std::string sopClassUid;

    if ( getInfo( fileList.front(), manufacturer, sopClassUid ) )
    {

      return soma::DicomReaderFactory::getInstance().read( manufacturer,
                                                           sopClassUid, 
                                                           fileList, 
                                                           proxy,
                                                           progress );

    }

  }

  return false;

}


bool soma::DicomIO::getInfo( const std::string& filename, 
                             std::string& manufacturer,
                             std::string& sopClassUid )
{

  DcmFileFormat fileFormat;

  manufacturer = "Generic";

  if ( fileFormat.loadFile( filename.c_str() ).good() )
  {

    OFString manufacturerStr;
    OFString sopUID;

    if ( fileFormat.getDataset()->findAndGetOFString( 
                                                     DCM_Manufacturer, 
                                                     manufacturerStr ).good() )
    {

      manufacturer = manufacturerStr.c_str();

    }

    if ( fileFormat.getDataset()->findAndGetOFString( DCM_SOPClassUID, 
                                                      sopUID ).good() )
    {

      sopClassUid = sopUID.c_str();

      return true;

    }

  }

  return false;

}
