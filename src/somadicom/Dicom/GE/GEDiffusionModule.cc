#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/GE/GEDiffusionModule.h>
#else
#include <Dicom/GE/GEDiffusionModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::GEDiffusionModule::GEDiffusionModule()
                       : soma::DiffusionModule()
{
}


bool soma::GEDiffusionModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    Sint32 tmpInt;
    
    if ( dataset->findAndGetSint32( DcmTagKey( 0x0043, 0x1039 ), 
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

        if ( dataset->findAndGetOFString( DCM_InPlanePhaseEncodingDirection,
                                          tmpString ).good() )
        {

          encodingDirection = tmpString.c_str();

        }

        if ( encodingDirection == "COL" )
        {

          if ( dataset->findAndGetFloat64( DcmTagKey( 0x0019, 0x10bb ),
                                           tmpDouble ).good() )
          {

            direction[ 0 ] = double( tmpDouble );

          }

          if ( dataset->findAndGetFloat64( DcmTagKey( 0x0019, 0x10bc ),
                                           tmpDouble ).good() )
          {

            direction[ 1 ] = double( tmpDouble );

          }

        }
        else if (  encodingDirection == "ROW" )
        {

          if ( dataset->findAndGetFloat64( DcmTagKey( 0x0019, 0x10bb ),
                                           tmpDouble ).good() )
          {

            direction[ 1 ] = double( tmpDouble );

          }

          if ( dataset->findAndGetFloat64( DcmTagKey( 0x0019, 0x10bc ),
                                           tmpDouble ).good() )
          {

            direction[ 0 ] = double( tmpDouble );

          }

        }

        if ( dataset->findAndGetFloat64( DcmTagKey( 0x0019, 0x10bd ),
                                         tmpDouble ).good() )
        {

          direction[ 2 ] = -1.0 * double( tmpDouble );

        }

      }

      _directions.push_back( direction );

      return true;

    }

  }

  return false;

}

