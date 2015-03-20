#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/NuclearModule.h>
#else
#include <Dicom/NuclearModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::NuclearModule::NuclearModule()
                   : soma::DicomModule(),
                     _scaleFactor( 1.0 ),
                     _numberOfTimeSlots( 0 ),
                     _numberOfSlices( 0 ),
                     _numberOfTimeSlices( 1 ),
                     _acquisitionMode( "" ),
                     _category( "" ),
                     _units( "" ),
                     _correctionMethod( "" ),
                     _reconstructionMethod( "" )
{

  // from DICOM 3 - Part 3 - C.8.9.1.1.3
  _unitNames.insert( std::make_pair( "CNTS", "counts" ) );
  _unitNames.insert( std::make_pair( "NONE", "unitless" ) );
  _unitNames.insert( std::make_pair( "CM2", "cm2" ) );
  _unitNames.insert( std::make_pair( "PCNT", "percent" ) );
  _unitNames.insert( std::make_pair( "CPS", "counts/s" ) );
  _unitNames.insert( std::make_pair( "BQML", "Bq/ml" ) );
  _unitNames.insert( std::make_pair( "MGMINML", "mg/min/ml" ) );
  _unitNames.insert( std::make_pair( "UMOLMINML", "uMol/min/ml" ) );
  _unitNames.insert( std::make_pair( "MLMING", "ml/min/g" ) );
  _unitNames.insert( std::make_pair( "MLG", "ml/g" ) );
  _unitNames.insert( std::make_pair( "1CM", "1/cm" ) );
  _unitNames.insert( std::make_pair( "UMOLML", "uMol/ml" ) );
  _unitNames.insert( std::make_pair( "PROPCNTS", "proportional to counts" ) );
  _unitNames.insert( std::make_pair( "PROPCPS", "proportional to counts/s" ) );
  _unitNames.insert( std::make_pair( "MLMINML", "ml/min/ml" ) );
  _unitNames.insert( std::make_pair( "MLML", "ml/ml" ) );
  _unitNames.insert( std::make_pair( "GML", "g/ml" ) );
  _unitNames.insert( std::make_pair( "STDDEV", "standard deviations" ) );

}


bool soma::NuclearModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    OFString tmpString;
    Uint16 tmpShort;

    if ( dcmItem->findAndGetUint16( DCM_NumberOfTimeSlots, tmpShort ).good() )
    {

      _numberOfTimeSlots = int32_t( tmpShort );

    }

    if ( dcmItem->findAndGetUint16( DCM_NumberOfSlices, tmpShort ).good() )
    {

      _numberOfSlices = int32_t( tmpShort );

    }

    if ( dcmItem->findAndGetUint16( DCM_NumberOfTimeSlices, tmpShort ).good() )
    {

      _numberOfTimeSlices = int32_t( tmpShort );

    }

    if ( dcmItem->findAndGetOFString( DCM_SeriesType, tmpString, 0 ).good() )
    {

      _acquisitionMode = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_SeriesType, tmpString, 1 ).good() )
    {

      _category = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_Units, tmpString ).good() )
    {

      _units = _unitNames[ tmpString.c_str() ];

    }

    _correctionMethod = "";

    if ( dcmItem->findAndGetOFString( DCM_RandomsCorrectionMethod, 
                                      tmpString ).good() )
    {

      _correctionMethod += tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_AttenuationCorrectionMethod, 
                                      tmpString ).good() )
    {

      if ( _correctionMethod.size() )
      {

        _correctionMethod += "\\";

      }

      _correctionMethod += tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_ScatterCorrectionMethod, 
                                      tmpString ).good() )
    {

      if ( _correctionMethod.size() )
      {

        _correctionMethod += "\\";

      }

      _correctionMethod += tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_ReconstructionMethod,
                                      tmpString ).good() )
    {

      _reconstructionMethod = tmpString.c_str();

    }

    return true;

  }

  return false;

}


double soma::NuclearModule::getScaleFactor() const
{

  return _scaleFactor;

}


int32_t soma::NuclearModule::getNumberOfTimeSlots() const
{

  return _numberOfTimeSlots;

}


int32_t soma::NuclearModule::getNumberOfSlices() const
{

  return _numberOfSlices;

}


int32_t soma::NuclearModule::getNumberOfTimeSlices() const
{

  return _numberOfTimeSlices;

}


const std::string& soma::NuclearModule::getAcquisitionMode() const
{

  return _acquisitionMode;

}


const std::string& soma::NuclearModule::getCategory() const
{

  return _category;

}


const std::string& soma::NuclearModule::getUnits() const
{

  return _units;

}


const std::string& soma::NuclearModule::getCorrectionMethod() const
{

  return _correctionMethod;

}


const std::string& soma::NuclearModule::getReconstructionMethod() const
{

  return _reconstructionMethod;

}

