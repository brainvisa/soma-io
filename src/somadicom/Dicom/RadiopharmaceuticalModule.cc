#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/RadiopharmaceuticalModule.h>
#else
#include <Dicom/RadiopharmaceuticalModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::RadiopharmaceuticalModule::RadiopharmaceuticalModule()
                               : soma::DicomModule(),
                                 _radiopharmaceutical( "" ),
                                 _radiopharmaceuticalStartTime( "" ),
                                 _radionuclideTotalDose( 0.0 ),
                                 _radionuclideHalfLife( 0.0 ),
                                 _radionuclidePositronFraction( 0.0 ),
                                 _radionuclide( "" )
{
}


bool soma::RadiopharmaceuticalModule::parseItem( DcmItem* dcmItem )
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
soma::RadiopharmaceuticalModule::getRadiopharmaceutical() const
{

  return _radiopharmaceutical;

}


const std::string&
soma::RadiopharmaceuticalModule::getRadiopharmaceuticalStartTime() const
{

  return _radiopharmaceuticalStartTime;

}


double soma::RadiopharmaceuticalModule::getRadionuclideTotalDose() const
{

  return _radionuclideTotalDose;

}


double soma::RadiopharmaceuticalModule::getRadionuclideHalfLife() const
{

  return _radionuclideHalfLife;

}


double soma::RadiopharmaceuticalModule::getRadionuclidePositronFraction() const
{

  return _radionuclidePositronFraction;

}


const std::string& soma::RadiopharmaceuticalModule::getRadionuclide() const
{

  return _radionuclide;

}

