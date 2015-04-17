#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GE/GEDiffusionModule.h>
#else
#include <Dicom/GE/GEDiffusionModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::GEDiffusionModule::GEDiffusionModule()
                      : dcm::DiffusionModule()
{
}


bool dcm::GEDiffusionModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    Sint32 tmpInt;
    
    if ( dcmItem->findAndGetSint32( DcmTagKey( 0x0043, 0x1039 ), 
                                    tmpInt, 
                                    0 ).good() )
    {

      std::vector< double > direction( 3, 0.0 );

      _bValues.push_back( double( tmpInt ) );

      if ( tmpInt > 0 )
      {

        OFString tmpString;
        Float64 tmpDouble;
        std::string encodingDirection = "COL";

        if ( dcmItem->findAndGetOFString( DCM_InPlanePhaseEncodingDirection,
                                          tmpString ).good() )
        {

          encodingDirection = tmpString.c_str();

        }

        if ( encodingDirection == "COL" )
        {

          if ( dcmItem->findAndGetFloat64( DcmTagKey( 0x0019, 0x10bb ),
                                           tmpDouble ).good() )
          {

            direction[ 0 ] = double( tmpDouble );

          }

          if ( dcmItem->findAndGetFloat64( DcmTagKey( 0x0019, 0x10bc ),
                                           tmpDouble ).good() )
          {

            direction[ 1 ] = double( tmpDouble );

          }

        }
        else if (  encodingDirection == "ROW" )
        {

          if ( dcmItem->findAndGetFloat64( DcmTagKey( 0x0019, 0x10bb ),
                                           tmpDouble ).good() )
          {

            direction[ 1 ] = double( tmpDouble );

          }

          if ( dcmItem->findAndGetFloat64( DcmTagKey( 0x0019, 0x10bc ),
                                           tmpDouble ).good() )
          {

            direction[ 0 ] = double( tmpDouble );

          }

        }

        if ( dcmItem->findAndGetFloat64( DcmTagKey( 0x0019, 0x10bd ),
                                         tmpDouble ).good() )
        {

          direction[ 2 ] = double( tmpDouble );

        }

      }

      addDirection( direction );

      return true;

    }

  }

  return false;

}
