#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Philips/PhilipsEnhancedIndexModule.h>
#else
#include <Dicom/Philips/PhilipsEnhancedIndexModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::PhilipsEnhancedIndexModule::PhilipsEnhancedIndexModule()
                                : soma::EnhancedIndexModule()
{
}


bool soma::PhilipsEnhancedIndexModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    int32_t nItems = 0;
    Sint32 tmpInt;
    DcmSequenceOfItems* seq = 0;

    if ( dataset->findAndGetSint32( DCM_NumberOfFrames, tmpInt ).good() )
    {

      _numberOfFrames = int32_t( tmpInt );

    }

    if ( dataset->findAndGetSint32( DcmTagKey( 0x2001, 0x1018 ), 
                                    tmpInt ).good() )
    {

      _zCount = int32_t( tmpInt );

    }

    _indices.resize( _numberOfFrames );

    if ( dataset->findAndGetSequence( DCM_DimensionIndexSequence,
                                      seq ).good() )
    {

      nItems = seq->card();

      if ( nItems < 4 )
      {

        return soma::EnhancedIndexModule::parseDataset( dataset );

      }
 
    }

    OFString tmpString;

    if ( dataset->findAndGetOFString( DCM_ImageType, tmpString, 2 ).good() )
    {

      if ( !tmpString.compare( "DIFFUSION" ) )
      {

        int32_t i;
        DcmTagKey inStackPos( DCM_InStackPositionNumber );
        DcmTagKey diffGradDir( DCM_DiffusionGradientOrientation );

        for ( i = 0; i < nItems; i++ )
        {

          DcmItem* item = seq->getItem( i );

          if ( item->findAndGetOFString( DCM_DimensionIndexPointer, 
                                         tmpString ).good() )
          {

            if ( !tmpString.compare( inStackPos.toString() ) )
            {

              _zIndex = i;

            }
            else if ( !tmpString.compare( diffGradDir.toString() ) )
            {

              _tIndex = i;

            }

          }

        }

        if ( _zIndex < 0 )
        {

          return false;

        }

        if ( dataset->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence,
                                          seq ).good() )
        {

          uint32_t i, nItems = seq->card();
          int32_t frames = _numberOfFrames / _zCount;

          for ( i = 0; i < nItems; i++ )
          {

            DcmItem* item = seq->getItem( i );
            DcmItem* tmpItem = 0;

            if ( item->findAndGetSequenceItem( DCM_FrameContentSequence,
                                               tmpItem ).good() )
            {

              Uint32 z, t;

              tmpItem->findAndGetUint32( DCM_DimensionIndexValues, z, _zIndex );
              tmpItem->findAndGetUint32( DCM_DimensionIndexValues, 
                                         t, 
                                         _tIndex );

              _indices[ i ] = ( t % frames ) * _zCount + z - 1;

            }

          }

        }
        else
        {

          int32_t i;

          for ( i = 0; i < _numberOfFrames; i++ )
          {

            _indices[ i ] = i;

          }

        }
 
        return true;

      }

    }

  }

  return false;

}

