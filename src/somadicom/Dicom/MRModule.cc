#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MRModule.h>
#else
#include <Dicom/MRModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::MRModule::MRModule()
             : dcm::DicomModule(),
               _tr( 0.0 ),
               _te( 0.0 ),
               _ti( 0.0 ),
               _flipAngle( 0.0 )
{
}


bool dcm::MRModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    Float64 tmpDouble;

    if ( dcmItem->findAndGetFloat64( DCM_RepetitionTime, tmpDouble ).good() )
    {

      _tr = double( tmpDouble );

    }

    if ( dcmItem->findAndGetFloat64( DCM_EchoTime, tmpDouble ).good() )
    {

      _te = double( tmpDouble );

    }

    if ( dcmItem->findAndGetFloat64( DCM_InversionTime, tmpDouble ).good() )
    {

      _ti = double( tmpDouble );

    }

    if ( dcmItem->findAndGetFloat64( DCM_FlipAngle, tmpDouble ).good() )
    {

      _flipAngle = double( tmpDouble );

    }

    return true;

  }

  return false;

}


double dcm::MRModule::getTR() const
{

  return _tr;

}


double dcm::MRModule::getTE() const
{

  return _te;

}


double dcm::MRModule::getTI() const
{

  return _ti;

}


double dcm::MRModule::getFlipAngle() const
{

  return _flipAngle;

}
