#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Philips/PhilipsEnhancedIndexModule.h>
#else
#include <Dicom/Philips/PhilipsEnhancedIndexModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::PhilipsEnhancedIndexModule::PhilipsEnhancedIndexModule()
                                : soma::EnhancedIndexModule()
{
}


bool soma::PhilipsEnhancedIndexModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    int32_t nItems = 0;
    DcmSequenceOfItems* seq = 0;

    if ( dcmItem->findAndGetSequence( DCM_DimensionIndexSequence,
                                      seq ).good() )
    {

      nItems = seq->card();

      if ( nItems < 4 )
      {

        return soma::EnhancedIndexModule::parseItem( dcmItem );

      }
 
    }

    Sint32 tmpInt;

    if ( dcmItem->findAndGetSint32( DCM_NumberOfFrames, tmpInt ).good() )
    {

      _numberOfFrames = int32_t( tmpInt );

    }
    else
    {

      return false;

    }

    if ( dcmItem->findAndGetSint32( DcmTagKey( 0x2001, 0x1018 ), 
                                    tmpInt ).good() )
    {

      _zCount = int32_t( tmpInt );

    }

    if ( _zCount < _numberOfFrames )
    {

      _tCount = _numberOfFrames / _zCount;

      if ( _numberOfFrames % _zCount )
      {

        _tCount++;

      }

    }

    _indices.resize( _numberOfFrames );

    OFString tmpString;

    if ( dcmItem->findAndGetOFString( DCM_ImageType, tmpString, 2 ).good() )
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

        if ( dcmItem->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence,
                                          seq ).good() )
        {

          uint32_t i, nItems = seq->card();

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

              _indices[ i ] = ( t % _tCount ) * _zCount + z - 1;

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

