#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/PositionModule.h>
#else
#include <Dicom/PositionModule.h>
#endif


#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <string>
#include <sstream>


dcm::PositionModule::PositionModule()
                   : dcm::DicomModule()
{
}


bool dcm::PositionModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    OFString tmpString;

    if ( dcmItem->findAndGetOFStringArray( DCM_ImagePositionPatient,
                                           tmpString ).good() )
    {

      std::string positionStr = tmpString.c_str();
      size_t pos = positionStr.find( "\\" );

      while ( pos != std::string::npos )
      {

        positionStr[ pos ] = ' ';
        pos = positionStr.find( "\\" );

      }

      std::istringstream iss( positionStr );

      iss >> _imagePosition.x
          >> _imagePosition.y
          >> _imagePosition.z;

      return true;

    }

  }

  return false;

}


dcm::Vector3d< double > dcm::PositionModule::getImagePosition() const
{

  return _imagePosition;

}
