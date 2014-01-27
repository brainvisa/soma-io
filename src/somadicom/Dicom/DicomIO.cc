#include <Dicom/DicomIO.h>
#include <Dicom/DicomReaderFactory.h>
#include <Dicom/DcmtkJpeg2000/djdecode.h>
#include <Container/Data.h>
#include <System/Directory.h>
#include <Pattern/Callback.h>

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmjpls/djdecode.h>

#include <sstream>
#include <cmath>
#include <map>


soma::DicomIO::DicomIO()
             : soma::Singleton< soma::DicomIO >()
{

  DcmRLEDecoderRegistration::registerCodecs();
  DJDecoderRegistration::registerCodecs();
  DJLSDecoderRegistration::registerCodecs();
  DJ2KDecoderRegistration::registerCodecs();

}


soma::DicomIO::~DicomIO()
{

  DJ2KDecoderRegistration::cleanup();
  DJLSDecoderRegistration::cleanup();
  DJDecoderRegistration::cleanup();
  DcmRLEDecoderRegistration::cleanup();

}


bool soma::DicomIO::read( const std::string& fileName, 
                          soma::Data& data,
                          soma::Callback* progress )
{

  soma::Directory directory( fileName );
  std::string selectedFile = directory.getSelectedFile();

  if ( !selectedFile.empty() )
  {

    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( fileName.c_str() ).good() )
    {

      OFString manufacturer;
      OFString sopUID;

      if ( fileFormat.getDataset()->findAndGetOFString( DCM_Manufacturer, 
                                                        manufacturer ).bad() )
      {

        manufacturer = "Generic";

      }

      if ( fileFormat.getDataset()->findAndGetOFString( DCM_SOPClassUID, 
                                                        sopUID ).good() )
      {

        return soma::DicomReaderFactory::getInstance().read( 
                                                           manufacturer.c_str(),
                                                           sopUID.c_str(), 
                                                           directory, 
                                                           data,
                                                           progress );

      }

    }

  }

  return false;

}
