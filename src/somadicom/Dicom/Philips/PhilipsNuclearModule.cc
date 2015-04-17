#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Philips/PhilipsNuclearModule.h>
#else
#include <Dicom/Philips/PhilipsNuclearModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::PhilipsNuclearModule::PhilipsNuclearModule()
                         : dcm::NuclearModule()
{
}


bool dcm::PhilipsNuclearModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    if ( dcm::NuclearModule::parseItem( dcmItem ) )
    {

      if ( _units == _unitNames[ "CNTS" ] )
      {

        Float64 tmpDouble;

        if ( dcmItem->findAndGetFloat64( DcmTagKey( 0x7053, 0x1009 ), 
                                         tmpDouble ).good() )
        {

          _units = _unitNames[ "BQML" ];
          _scaleFactor = double( tmpDouble );

        }

      }

      return true;

    }

  }

  return false;

}
