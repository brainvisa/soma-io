#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Philips/PhilipsNuclearModule.h>
#else
#include <Dicom/Philips/PhilipsNuclearModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::PhilipsNuclearModule::PhilipsNuclearModule()
                          : soma::NuclearModule()
{
}


bool soma::PhilipsNuclearModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    if ( soma::NuclearModule::parseDataset( dataset ) )
    {

      if ( _units == _unitNames[ "CNTS" ] )
      {

        Float64 tmpDouble;

        if ( dataset->findAndGetFloat64( DcmTagKey( 0x7053, 0x1009 ), 
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

