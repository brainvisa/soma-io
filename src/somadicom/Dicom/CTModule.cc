#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/CTModule.h>
#else
#include <Dicom/CTModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::CTModule::CTModule()
             : dcm::DicomModule(),
               _reconstructionDiameter( 0.0 )
{
}


bool dcm::CTModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    Float64 tmpDouble;

    if ( dcmItem->findAndGetFloat64( DCM_ReconstructionDiameter, 
                                     tmpDouble ).good() )
    {

      _reconstructionDiameter = double( tmpDouble );

    }

    return true;

  }

  return false;

}


double dcm::CTModule::getReconstructionDiameter() const
{

  return _reconstructionDiameter;

}
