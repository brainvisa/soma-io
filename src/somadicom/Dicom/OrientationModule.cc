#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/OrientationModule.h>
#else
#include <Dicom/OrientationModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <string>
#include <sstream>


dcm::OrientationModule::OrientationModule()
                      : dcm::DicomModule()
{
}


bool dcm::OrientationModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    OFString tmpString;

    if ( dcmItem->findAndGetOFStringArray( DCM_ImageOrientationPatient,
                                           tmpString ).good() )
    {

      std::string orientationStr = tmpString.c_str();
      size_t pos = orientationStr.find( "\\" );

      while ( pos != std::string::npos )
      {

        orientationStr[ pos ] = ' ';
        pos = orientationStr.find( "\\" );

      }

      std::istringstream iss( orientationStr );

      iss >> _rowCosine.x
          >> _rowCosine.y
          >> _rowCosine.z
          >> _columnCosine.x
          >> _columnCosine.y
          >> _columnCosine.z;

      return true;

    }

  }

  return false;

}


bool dcm::OrientationModule::parseDataset( DcmDataset* dataset )
{

  return parseItem( dataset );

}


dcm::Vector3d< double > dcm::OrientationModule::getRowCosine() const
{

  return _rowCosine;

}


dcm::Vector3d< double > dcm::OrientationModule::getColumnCosine() const
{

  return _columnCosine;

}
