#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MRModule.h>
#else
#include <Dicom/MRModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::MRModule::MRModule()
              : soma::DicomModule(),
                _tr( 0.0 ),
                _te( 0.0 ),
                _ti( 0.0 ),
                _flipAngle( 0.0 )
{
}


bool soma::MRModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    Float64 tmpDouble;

    if ( dataset->findAndGetFloat64( DCM_RepetitionTime, tmpDouble ).good() )
    {

      _tr = double( tmpDouble );

    }

    if ( dataset->findAndGetFloat64( DCM_EchoTime, tmpDouble ).good() )
    {

      _te = double( tmpDouble );

    }

    if ( dataset->findAndGetFloat64( DCM_InversionTime, tmpDouble ).good() )
    {

      _ti = double( tmpDouble );

    }

    if ( dataset->findAndGetFloat64( DCM_FlipAngle, tmpDouble ).good() )
    {

      _flipAngle = double( tmpDouble );

    }

    return true;

  }

  return false;

}


double soma::MRModule::getTR() const
{

  return _tr;

}


double soma::MRModule::getTE() const
{

  return _te;

}


double soma::MRModule::getTI() const
{

  return _ti;

}


double soma::MRModule::getFlipAngle() const
{

  return _flipAngle;

}

