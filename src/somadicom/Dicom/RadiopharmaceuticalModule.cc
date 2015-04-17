#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/RadiopharmaceuticalModule.h>
#else
#include <Dicom/RadiopharmaceuticalModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::RadiopharmaceuticalModule::RadiopharmaceuticalModule()
                              : dcm::DicomModule(),
                                _radiopharmaceutical( "" ),
                                _radiopharmaceuticalStartTime( "" ),
                                _radionuclideTotalDose( 0.0 ),
                                _radionuclideHalfLife( 0.0 ),
                                _radionuclidePositronFraction( 0.0 ),
                                _radionuclide( "" )
{
}


bool dcm::RadiopharmaceuticalModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    DcmItem* item = 0;
    if ( dcmItem->findAndGetSequenceItem( 
                                     DCM_RadiopharmaceuticalInformationSequence,
                                     item ).good() )
    {

      OFString tmpString;
      Float64 tmpDouble;
      DcmItem* rnItem = 0;

      if ( item->findAndGetOFString( DCM_Radiopharmaceutical,
                                     tmpString ).good() )
      {

        _radiopharmaceutical = tmpString.c_str();

      }

      if ( item->findAndGetOFString( DCM_RadiopharmaceuticalStartTime,
                                     tmpString ).good() )
      {

        _radiopharmaceuticalStartTime = tmpString.c_str();

      }

      if ( item->findAndGetFloat64( DCM_RadionuclideTotalDose,
                                    tmpDouble ).good() )
      {

        _radionuclideTotalDose = double( tmpDouble );

      }

      if ( item->findAndGetFloat64( DCM_RadionuclideHalfLife,
                                    tmpDouble ).good() )
      {

        _radionuclideHalfLife = double( tmpDouble );

      }

      if ( item->findAndGetFloat64( DCM_RadionuclidePositronFraction,
                                    tmpDouble ).good() )
      {

        _radionuclidePositronFraction = double( tmpDouble );

      }

      if ( item->findAndGetSequenceItem( DCM_RadionuclideCodeSequence,
                                         rnItem ).good() )
      {

        if ( rnItem->findAndGetOFString( DCM_CodeMeaning, tmpString ).good() )
        {

          _radionuclide = tmpString.c_str();

        }

      }

    }
  
    return true;

  }

  return false;

}


const std::string& 
dcm::RadiopharmaceuticalModule::getRadiopharmaceutical() const
{

  return _radiopharmaceutical;

}


const std::string&
dcm::RadiopharmaceuticalModule::getRadiopharmaceuticalStartTime() const
{

  return _radiopharmaceuticalStartTime;

}


double dcm::RadiopharmaceuticalModule::getRadionuclideTotalDose() const
{

  return _radionuclideTotalDose;

}


double dcm::RadiopharmaceuticalModule::getRadionuclideHalfLife() const
{

  return _radionuclideHalfLife;

}


double dcm::RadiopharmaceuticalModule::getRadionuclidePositronFraction() const
{

  return _radionuclidePositronFraction;

}


const std::string& dcm::RadiopharmaceuticalModule::getRadionuclide() const
{

  return _radionuclide;

}
